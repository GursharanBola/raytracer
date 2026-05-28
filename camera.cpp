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

    enum CameraType { CAMERA_FLAT = 0, CAMERA_SPHERICAL = 1 };
    CameraType camera_type;
    double focal_length;
    double pi = 3.1415926535897932;
    camera(const vec3 &camera_center, CameraType type, double f_length = 1.0)
        : center(camera_center), camera_type(type), focal_length(f_length) {}

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
        case CAMERA_SPHERICAL: {
            double delta_theta = pi / (image_height - 1);
            double delta_phi = (2 * pi) / (image_width - 1);
            vec3 start_ver_cir = vec3{0, focal_length, 0};

            for (int i = 0; i < image_height; i++) {
                vec3 curr_ver_cir = start_ver_cir.rotate_x(delta_theta * i);

                for (int j = 0; j < image_width; j++) {

                    vec3 curr_hor_cir = curr_ver_cir.rotate_y(delta_phi * j);

                    vec3 average_color = average_pixel_angular(
                        i, j, delta_theta, delta_phi, start_ver_cir, world,
                        aliasing_samples, center);

                    image.set_color(j, i,
                                    average_color); // TODO: maybe check if
                                                    // set_color() fails.
                }
            }
            break;
        }
        case CAMERA_FLAT: {
            double aspect_ratio = (double)image_width / image_height;
            for (int i = 0; i < image_height; i++) {
                for (int j = 0; j < image_width; j++) {

                    double percent_x = (j + 0.5) / image_width;
                    double percent_y = (i + 0.5) / image_height;

                    double screen_x =
                        center.vec[0] + (2.0 * percent_x - 1.0) * aspect_ratio;
                    double screen_y = center.vec[1] - (2.0 * percent_y - 1.0);
                    double screen_z = center.vec[2] - focal_length;

                    vec3 screen_point = vec3{screen_x, screen_y, screen_z};
                    vec3 dir = unit_vector(screen_point - center);

                    vec3 avg_color = average_pixel_linear(
                        i, j, screen_z, world, aliasing_samples, center,
                        image_width, image_height);

                    image.set_color(j, i, avg_color);
                }
            }
            break;
        }
        }
    }

    // This is adding noise on the angle we are sampling that is why this
    // differs from the linear option
    vec3 average_pixel_angular(int i, int j, double delta_theta,
                               double delta_phi, const vec3 &start_ver_cir,
                               const hittable_list &world, int num_samples,
                               vec3 camera_center) const {

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

            ray jittered_ray = ray(camera_center, jittered_dir);
            vec3 sample_color = color(jittered_ray, world);

            avg_color += sample_color;
        }
        return avg_color / num_samples;
    }
    vec3 average_pixel_linear(int i, int j, double screen_z,
                              const hittable_list &world, int num_samples,
                              vec3 camera_center, int image_width,
                              int image_height) const {
        vec3 avg_color = vec3{0, 0, 0};

        double aspect_ratio = (double)image_width / image_height;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-0.5, 0.5);

        for (int k = 0; k < num_samples; k++) {

            double jit_i = i + dis(gen);
            double jit_j = j + dis(gen);

            double jit_screen_x =
                ((jit_j / (image_width - 1)) * 2.0 - 1.0) * aspect_ratio;
            double jit_screen_y = (jit_i / (image_height - 1)) * 2.0 - 1.0;

            vec3 jittered_dir = vec3{jit_screen_x, jit_screen_y, screen_z};

            ray jittered_ray = ray(camera_center, unit_vector(jittered_dir));

            avg_color += color(jittered_ray, world);
        }

        return avg_color / num_samples;
    }

    // TODO: Next to impliment and refine.

    vec3 color(const ray &r, const hittable_list &world, double ray_tmin,
               double ray_tmax, int depth) const {
        hit_record rec;
        if (world.hit(r, ray_tmin, ray_tmax, rec)) {
            return vec3{1, 1, 1};
        }
        // NOTE: bounce() always returns the a normal vector AT the point of
        // intersection
        vec3 new_dir =
            rec.mat->bounce(rec.point, rec.normal, world, r.direction());
        ray new_ray = ray(rec.point, new_dir);

        return elem_mul(rec.mat->color,
                        color(new_ray, world, ray_tmin, ray_tmax, depth - 1));
    }
};

#endif
