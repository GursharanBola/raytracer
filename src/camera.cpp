#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "img.h"
#include "material.h"
#include "vec3.h"
#include <cmath>
#include <random>
#include <stdexcept>

/* Welcome to the Camera! */

// NOTE: This is for varied image sizes which isn't supported yet so we
// should stick to inputting image_width and img_height as it is hard coded
// int the img headerfile.
void camera::render(const hittable_list &world, int image_width,
                    int image_height, img &image) {
    switch (camera_type) {
    case camera::CAMERA_SPHERICAL: {
        // NOTE: Theta is the vertical and phi is the horizontal.
        double delta_theta = pi / (image_height - 1);
        double delta_phi = (2 * pi) / (image_width - 1);
        // vec3 start_ver_cir = vec3{0, focal_length, 0};
        for (int i = 0; i < image_height; i++) {
            // vec3 curr_ver_cir = start_ver_cir.rotate_x(delta_theta * i);
            for (int j = 0; j < image_width; j++) {
                // vec3 curr_hor_cir = curr_ver_cir.rotate_y(delta_phi * j);
                vec3 average_color =
                    average_pixel_angular(i, j, delta_theta, delta_phi, world);
                bool valid = image.set_color(j, i, average_color);
                if (!valid) {
                    throw std::runtime_error("Somehow failed to set color!");
                }
            }
        }
        break;
    }
    case camera::CAMERA_FLAT: {
        double aspect_ratio = (double)image_width / image_height;
        for (int i = 0; i < image_height; i++) {
            for (int j = 0; j < image_width; j++) {

                vec3 avg_color = average_pixel_linear(i, j, world, image_width,
                                                      image_height);
                bool valid = image.set_color(j, i, avg_color);
                if (!valid) {
                    throw std::runtime_error("Somehow failed to set color!");
                }
            }
        }
        break;
    }
        // NOTE: Fisheye lenses "require" the image be square to perfectly fit a
        // cricle.

    case camera::CAMERA_FISHEYE: {
        double fisheye_radius = 1.0;
        for (int i = 0; i < image_height; i++) {
            for (int j = 0; j < image_width; j++) {
                vec3 avg_color = average_pixel_fisheye(
                    i, j, world, image_width, image_height, fisheye_radius);

                bool valid = image.set_color(j, i, avg_color);

                if (!valid) {
                    throw std::runtime_error("Somehow failed to set color!");
                }
            }
        }
    }
    }
}

vec3 camera::average_pixel_fisheye(int i, int j, const hittable_list &world,
                                   int image_width, int image_height,
                                   double rad) const {
    double aspect_ratio = (double)image_width / image_height;

    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.5, 0.5);

    vec3 avg_color = vec3{0, 0, 0};

    for (int k = 0; k < num_samples; k++) {

        double jit_i = i + dis(gen);
        double jit_j = j + dis(gen);

        double percent_x = jit_j / image_width;
        double percent_y = jit_i / image_height;

        double jit_screen_x = (2.0 * percent_x - 1.0) * aspect_ratio;
        double jit_screen_y = -(2.0 * percent_y - 1.0);

        double x_sq = jit_screen_x * jit_screen_x;
        double y_sq = jit_screen_y * jit_screen_y;
        double rad_sq = rad * rad;

        double dist = x_sq + y_sq;

        if (dist > rad_sq) {
            continue; // don't add anything to the color
        }

        double ref_theta = std::sqrt(dist) / focal_dist;
        double ref_phi = std::atan2(jit_screen_y, jit_screen_x);

        double dx = std::sin(ref_theta) * std::cos(ref_phi);
        double dy = std::sin(ref_theta) * std::sin(ref_phi);
        double dz = std::cos(ref_theta);

        // dx, dy, and dz are in local space.
        vec3 global_dir = (dx * cam_u) + (dy * cam_v) - (dz * cam_w);

        ray r = ray(center, global_dir);

        vec3 sample_color = color(r, world, depth);

        avg_color += sample_color;
    }
    return gamma_correct(avg_color / num_samples);
}

// NOTE: This is supporting anti-aliasing and works for spherical lenses
// That CAN be rotated. Bokeh is not supported for spherical lenses.
vec3 camera::average_pixel_angular(int i, int j, double delta_theta,
                                   double delta_phi,
                                   const hittable_list &world) const {

    vec3 avg_color = vec3{0, 0, 0};

    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.5, 0.5);

    for (int k = 0; k < num_samples; k++) {

        double random_t = dis(gen);
        double random_p = dis(gen);
        double jit_theta = delta_theta * (i + random_t);
        double jit_phi = delta_phi * (j + random_p) - pi;

        // Rotation of vector.
        // jittered_dir =
        // start_ver_cir.rotate_ver(jit_theta).rotate_hor(jit_phi);
        double sin_theta = std::sin(jit_theta);
        double cos_theta = std::cos(jit_theta);
        double sin_phi = std::sin(jit_phi);
        double cos_phi = std::cos(jit_phi);

        vec3 jittered_dir = vec3(sin_theta * (std::sin(jit_phi)), // X component
                                 cos_theta,                       // Y component
                                 sin_theta * (std::cos(jit_phi))  // Z component
        );

        vec3 global_jittered_dir = (jittered_dir.x() * cam_u) +
                                   (jittered_dir.y() * cam_v) -
                                   (jittered_dir.z() * cam_w);

        global_jittered_dir =
            global_jittered_dir / global_jittered_dir.length();

        ray jittered_ray = ray(center, global_jittered_dir);

        vec3 sample_color = color(jittered_ray, world, depth);

        avg_color += sample_color;
    }

    avg_color = avg_color / num_samples;

    return gamma_correct(avg_color);
}

// NOTE: Flat lenses do have Bokeh, Anti Aliasing, can be rotated, and
// shifted.
vec3 camera::average_pixel_linear(int i, int j, const hittable_list &world,
                                  int image_width, int image_height) const {
    vec3 avg_color = vec3{0, 0, 0};

    double aspect_ratio = (double)image_width / image_height;

    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.5, 0.5);

    for (int k = 0; k < num_samples; k++) {

        double jit_i = i + dis(gen);
        double jit_j = j + dis(gen);

        double percent_x = jit_j / image_width;
        double percent_y = jit_i / image_height;

        double jit_screen_x = (2.0 * percent_x - 1.0) * aspect_ratio;
        double jit_screen_y = -(2.0 * percent_y - 1.0);

        // TODO: Next to verify if working.
        // NOTE: Bokeh vvv
        vec3 random_unit_disk = random_vec3(-aperture_radius, aperture_radius);
        random_unit_disk.vec[2] = 0;

        while (dot(random_unit_disk, random_unit_disk) >
               aperture_radius * aperture_radius) {
            random_unit_disk = random_vec3(-aperture_radius, aperture_radius);
            random_unit_disk.vec[2] = 0;
        }

        double inter_p = focal_dist / focal_length;

        vec3 P_focus_local =
            vec3(jit_screen_x * inter_p, jit_screen_y * inter_p, -focal_dist);

        vec3 local_dir = P_focus_local - random_unit_disk;

        vec3 world_dir = local_dir.x() * cam_u + local_dir.y() * cam_v +
                         local_dir.z() * cam_w;

        vec3 world_lens_offset =
            random_unit_disk.x() * cam_u + random_unit_disk.y() * cam_v;
        vec3 ray_origin = center + world_lens_offset;

        ray jittered_ray = ray(ray_origin, unit_vector(world_dir));

        vec3 sample_color = color(jittered_ray, world, depth);

        avg_color += sample_color;
    }

    avg_color = avg_color / num_samples;

    return gamma_correct(avg_color);
}

vec3 camera::color(const ray &r, const hittable_list &world, int depth) const {
    if (depth <= 0) {
        return vec3{0, 0, 0}; // shadows are dark
    }

    hit_record rec;
    if (!world.hit(r, t_min, t_max, rec)) {
        vec3 u_dir = r.direction() / r.direction().length();
        // NOTE: If you want to change the background color then we change
        // it here: For now the setting is OUTSIDE, that is under natural
        // sky.
        vec3 light_blue = vec3{0, 0, 0.5};
        vec3 white = vec3{.9, .9, .9};

        double normalized_y = 0.5 * (u_dir.y() + 1.0);

        return vec3{0, 0, 0};
        // NOTE: If you want the sky back uncomment this line
        // return (1 - normalized_y) * white + light_blue * normalized_y;
    }
    // Terminate early if we hit a light.
    if (rec.mat->is_light()) {
        return rec.mat->color;
    }

    // NOTE: bounce() must always return the direction.
    vec3 new_dir =
        rec.mat->bounce(rec.point, rec.normal, r.direction(), rec.front_face);
    double s = 1e-8; // check for dead rays.
    if ((std::fabs(new_dir.vec[0]) < s) && (std::fabs(new_dir.vec[1]) < s) &&
        (std::fabs(new_dir.vec[2]) < s)) {
        return vec3{0, 0, 0};
    }
    ray new_ray = ray(rec.point, new_dir);

    vec3 single_color = color(new_ray, world, depth - 1);

    return elem_mul(rec.mat->color, single_color);
}

vec3 camera::gamma_correct(const vec3 &color) const {
    return vec3(std::sqrt(color.x()), std::sqrt(color.y()),
                std::sqrt(color.z()));
}
