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
    camera(const vec3 &camera_center, CameraType type, double f_length = 1.0,
           vec3 u_axis = vec3(1, 0, 0), vec3 v_axis = vec3(0, 1, 0),
           vec3 w_axis = vec3(0, 0, 1))
        : center(camera_center), camera_type(type), focal_length(f_length),
          cam_u(u_axis), cam_v(v_axis), cam_w(w_axis) {}

    // NOTE: This is for varied image sizes which isn't supported yet so we
    // should stick to inputting image_width and img_height as it is hard coded
    // int the img headerfile.
    void render(const hittable_list &world, int image_width, int image_height) {
        img image = img{}; // create a width 800, height 600 image with 3 8 bit
                           // channels.

        // NOTE: These are the intervals we can adjust to determine the depth of
        // the ray
        int t_max = 100;
        int t_min = 0;

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

                    image.set_color(j, i,
                                    average_color); // TODO: maybe check if
                                                    // set_color() fails.
                }
            }
            break;
        }
        // TODO: Handle angling the plane for the angle that we are looking at
        // the scene with.
        case CAMERA_FLAT: {
            double aspect_ratio = (double)image_width / image_height;
            for (int i = 0; i < image_height; i++) {
                for (int j = 0; j < image_width; j++) {
                    double apeture_radius = 0.5;

                    vec3 avg_color = average_pixel_linear(
                        i, j, -focal_length, world, aliasing_samples, center,
                        image_width, image_height, apeture_radius, cam_u, cam_v,
                        cam_w);

                    image.set_color(j, i, avg_color);
                }
            }
            break;
        }
        }
        // TODO: OPTIONAL: Create a Fisheye a large cut out of a sphere
    }

    /*------------------------------- DESIGN -----------------------------*/
    /*  Bokeh, camera rotation, and antialiasing is supported for flat sensors.
     * In order to do this in, average_pixel_linear() I will jitter start of the
     * shooting ray to be within a disk (standard x^2 +y^2 = 1) and then rotate
     * it: pitch, yaw, and roll by doing the opperation one Line 140 and adding
     * it to the start of the ray.
     *
     * Antialiasing will be done by doing sub_pixel jittering (i +- .5 and j +-
     * .5) and sampling multiple times. Notice that we are doing both
     * opperations at the same time i.e.: O(s) time.
     */

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

            // NOTE: This is the subpixel jittering
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

            int depth = 10;

            vec3 sample_color = color(jittered_ray, world, t_min, t_max, depth);

            avg_color += sample_color;
        }
        return avg_color / num_samples;
    }

    // TODO: Support sub_pixel jittering as well as bokeh WITH angled focal
    // plane.
    vec3 average_pixel_linear(int i, int j, double screen_z,
                              const hittable_list &world, int num_samples,
                              const vec3 &camera_center, int image_width,
                              int image_height, double aperture_radius,
                              const vec3 &cam_u, const vec3 &cam_v,
                              const vec3 &cam_w) const {
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

            vec3 rotated_and_jittered_dir = (jit_screen_x * cam_u) +
                                            (jit_screen_y * cam_v) +
                                            (screen_z * cam_w);

            vec3 rotated_shifted_and_jittered_dir =
                rotated_and_jittered_dir + center;

            ray jittered_ray =
                ray(camera_center, unit_vector(rotated_and_jittered_dir));

            double t_min = 0.0001; // This is going to be used so that we can
                                   // avoid shadow acne
            double t_max = std::numeric_limits<double>::max();
            double depth = 10; // limit to 10 bounces

            avg_color += color(jittered_ray, world, t_min, t_max, depth);
        }

        return avg_color / num_samples;
    }

    // TODO: Impliment lighting and see how they can be rendered on a scene.
    // NOTE: ray_tmin and ray_tmax should be somethig around 0.001, infinity
    // So that we can no shadow acne, but again that is up the user
    vec3 color(const ray &r, const hittable_list &world, double ray_tmin,
               double ray_tmax, int depth) const {
        if (depth <= 0) {
            return vec3{0, 0, 0}; // shadows are dark
        }

        hit_record rec;
        if (!world.hit(r, ray_tmin, ray_tmax, rec)) {
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
        vec3 new_dir = rec.mat->bounce(rec.point, rec.normal, r.direction());
        double s = 1e-8; // check for dead rays.
        if ((std::fabs(new_dir.vec[0]) < s) &&
            (std::fabs(new_dir.vec[1]) < s) &&
            (std::fabs(new_dir.vec[2]) < s)) {
            return vec3{0, 0, 0};
        }
        ray new_ray = ray(rec.point, new_dir);

        return elem_mul(rec.mat->color,
                        color(new_ray, world, ray_tmin, ray_tmax, depth - 1));
    }
};

#endif
