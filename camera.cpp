#ifndef CAMERA
#define CAMERA

#include "hittable.h"
#include "hittable_list.h"
#include "img.h"
#include "material.h"
#include "vec3.h"
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

/**
 * For this project the camera type will determine the rendering that we will
 * see the render() function will render the objects by calling color() as a
 * subroutine. color() will take in a ray, and return the color of the
 * associated pixel given a world. color() will take into account the
 * surface_type which is given in the METADATA of the object type.
 */

class camera {
  public:
    vec3 center;
    vec3 cam_u; // right
    vec3 cam_v; // up
    vec3 cam_w; // backward
    enum CameraType { CAMERA_FLAT = 0, CAMERA_SPHERICAL = 1 };
    CameraType camera_type;
    double focal_length;
    double pi = 3.1415926535897932;
    double focal_dist;
    double apeture_radius = 0.5;
    double t_max;
    double t_min;
    camera(const vec3 &camera_center, CameraType type, double f_dist,
           double tray_max = std::numeric_limits<double>::max(),
           double tray_min = 0.001, double f_length = 1.0,
           vec3 u_axis = vec3(1, 0, 0), vec3 v_axis = vec3(0, 1, 0),
           vec3 w_axis = vec3(0, 0, 1))
        : center(camera_center), cam_u(u_axis), cam_v(v_axis), cam_w(w_axis),
          camera_type(type), focal_length(f_length), focal_dist(f_dist),
          t_max(tray_max), t_min(tray_min) {}

    // NOTE: This is for varied image sizes which isn't supported yet so we
    // should stick to inputting image_width and img_height as it is hard coded
    // int the img headerfile.
    void render(const hittable_list &world, int image_width, int image_height,
                double t_max, double t_min) {
        img image = img{}; // create a width 800, height 600 image with 3 8 bit
                           // channels.

        int aliasing_samples = 3;

        switch (camera_type) {
        // NOTE: This works with rotating the camera using cam_u, cam_v, and
        // cam_w.
        case CAMERA_SPHERICAL: {
            double delta_theta = pi / (image_height - 1);
            double delta_phi = (2 * pi) / (image_width - 1);
            vec3 start_ver_cir = vec3{0, focal_length, 0};

            for (int i = 0; i < image_height; i++) {
                vec3 curr_ver_cir = start_ver_cir.rotate_x(delta_theta * i);

                for (int j = 0; j < image_width; j++) {

                    // vec3 curr_hor_cir = curr_ver_cir.rotate_y(delta_phi * j);

                    vec3 average_color = average_pixel_angular(
                        i, j, delta_theta, delta_phi, start_ver_cir, world,
                        aliasing_samples, center, cam_u, cam_v, cam_w);

                    image.set_color(
                        j, i,
                        average_color); // TODO: check if set_color() fails.
                }
            }
            break;
        }
        case CAMERA_FLAT: {
            double aspect_ratio = (double)image_width / image_height;
            for (int i = 0; i < image_height; i++) {
                for (int j = 0; j < image_width; j++) {

                    vec3 avg_color = average_pixel_linear(
                        i, j, world, aliasing_samples, image_width,
                        image_height, apeture_radius, cam_u, cam_v, cam_w,
                        focal_dist);

                    image.set_color(j, i, avg_color);
                }
            }
            break;
        }
        }
        // TODO: Add a fisheye lens using a section of a large sphere as the
        // image plane
    }

    // NOTE: This is supporting anti-aliasing and works for spherical lenses
    // That CAN be rotated. Bokeh is not supported for spherical lenses.
    vec3 average_pixel_angular(int i, int j, double delta_theta,
                               double delta_phi, const vec3 &start_ver_cir,
                               const hittable_list &world, int num_samples,
                               vec3 camera_center, vec3 cam_u, vec3 cam_v,
                               vec3 cam_w) const {

        vec3 avg_color = vec3{0, 0, 0};

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-0.5, 0.5);

        for (int k = 0; k < num_samples; k++) {

            double random_t = dis(gen);
            double random_p = dis(gen);
            double jit_theta = delta_theta * (i + random_t);
            double jit_phi = delta_phi * (j + random_p);

            vec3 jittered_dir =
                start_ver_cir.rotate_x(jit_theta).rotate_y(jit_phi);

            vec3 rotated_and_jittered_dir = (jittered_dir.x() * cam_u) +
                                            (jittered_dir.y() * cam_v) -
                                            (jittered_dir.z() * cam_w);

            rotated_and_jittered_dir =
                rotated_and_jittered_dir / rotated_and_jittered_dir.length();

            ray jittered_ray = ray(camera_center, rotated_and_jittered_dir);

            double t_min = 0.0001;
            double t_max = std::numeric_limits<double>::max();

            int depth = 10; // limit to 10 bounces.

            vec3 sample_color = color(jittered_ray, world, depth);

            avg_color += sample_color;
        }
        return avg_color / num_samples;
    }

    // NOTE: Flat lenses do have Bokeh, Anti Aliasing, can be rotated, and
    // shifted.
    vec3 average_pixel_linear(int i, int j, const hittable_list &world,
                              int num_samples, int image_width,
                              int image_height, double aperture_radius,
                              const vec3 &cam_u, const vec3 &cam_v,
                              const vec3 &cam_w, double focal_dist) const {
        vec3 avg_color = vec3{0, 0, 0};

        double aspect_ratio = (double)image_width / image_height;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-0.5, 0.5);

        for (int k = 0; k < num_samples; k++) {

            double jit_i = i + dis(gen);
            double jit_j = j + dis(gen);

            double percent_x = jit_j / image_width;
            double percent_y = jit_i / image_height;

            double jit_screen_x = (2.0 * percent_x - 1.0) * aspect_ratio;
            double jit_screen_y = -(2.0 * percent_y - 1.0);

            // NOTE: Bokeh vvv
            vec3 random_unit_disk =
                random_vec3(-aperture_radius, aperture_radius);
            random_unit_disk.vec[2] = 0;

            while (dot(random_unit_disk, random_unit_disk) >
                   aperture_radius * aperture_radius) {
                random_unit_disk =
                    random_vec3(-aperture_radius, aperture_radius);
                random_unit_disk.vec[2] = 0;
            }

            double inter_p = focal_dist / focal_length;

            vec3 P_focus_local = vec3(jit_screen_x * inter_p,
                                      jit_screen_y * inter_p, -focal_dist);

            vec3 local_dir = P_focus_local - random_unit_disk;

            vec3 world_dir = local_dir.x() * cam_u + local_dir.y() * cam_v +
                             local_dir.z() * cam_w;

            vec3 world_lens_offset =
                random_unit_disk.x() * cam_u + random_unit_disk.y() * cam_v;
            vec3 ray_origin = center + world_lens_offset;

            ray jittered_ray = ray(ray_origin, unit_vector(world_dir));

            double depth = 10; // limit to 10 bounces

            avg_color += color(jittered_ray, world, depth);
        }

        return avg_color / num_samples;
    }

    // TODO: Impliment lighting and see how they can be rendered on a scene.
    vec3 color(const ray &r, const hittable_list &world, int depth) const {
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

            return (1 - normalized_y) * white + light_blue * normalized_y;
        }

        // NOTE: bounce() must always return the direction.
        vec3 new_dir = rec.mat->bounce(rec.point, rec.normal, r.direction(),
                                       rec.front_face);
        double s = 1e-8; // check for dead rays.
        if ((std::fabs(new_dir.vec[0]) < s) &&
            (std::fabs(new_dir.vec[1]) < s) &&
            (std::fabs(new_dir.vec[2]) < s)) {
            return vec3{0, 0, 0};
        }
        ray new_ray = ray(rec.point, new_dir);

        return elem_mul(rec.mat->color, color(new_ray, world, depth - 1));
    }
};

#endif
