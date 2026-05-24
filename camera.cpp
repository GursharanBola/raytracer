#ifndef CAMERA
#define CAMERA

#include "hittable_list.h"
#include "img.h"
#include "vec3.h"
#include <cmath>
#include <iostream>
#include <vector>

/**
 * Although the camera is what decides how we look at our scene we should also
 * put decide that this is where of what type of focal plane, spherical,
 * cylindrical, or maybe even a custom functionality, where we can input a
 * function.
 *
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
        int t_max = 100;
        int t_min = 0;
        switch (camera_type) {
        case CAMERA_SPHERICAL: {
            vec3 curr_ver_cir =
                vec3{0, focal_length, 0}; // start at the top of the sphere
            double delta_theta = pi / (image_height - 1);
            double delta_phi = (2 * pi) / (image_width - 1);

            for (int i = 0; i < image_height; i++) {

                double x_k = curr_ver_cir.vec[0];
                double y_k = curr_ver_cir.vec[1];
                double z_k = curr_ver_cir.vec[2];

                vec3 curr_hor_cir = vec3{z_k, y_k, 0.0};
                for (int j = 0; j < image_width; j++) {
                    ray r = ray(center, unit_vector(curr_hor_cir - center));

                    // NOTE: call color() here which will take in the ray
                    // and return the color that we are looking for.

                    double x_k = curr_hor_cir.vec[0];
                    double y_k = curr_hor_cir.vec[1];
                    double z_k = curr_hor_cir.vec[2];

                    double x_next =
                        x_k * std::cos(delta_phi) - z_k * std::sin(delta_phi);
                    double z_next =
                        x_k * std::sin(delta_phi) + z_k * std::cos(delta_phi);

                    curr_hor_cir = vec3{x_next, y_k, z_next};
                }
                double y_next =
                    y_k * std::cos(delta_theta) - z_k * std::sin(delta_theta);
                double z_next =
                    y_k * std::sin(delta_theta) + z_k * std::cos(delta_theta);

                curr_ver_cir = vec3{0.0, y_next, z_next};
            }
            break;
        }

        case CAMERA_FLAT: {
            double aspect_ratio = (double)image_width / image_height;
            for (int i = 0; i < image_height; i++) {
                for (int j = 0; j < image_width; j++) {
                    // the focal plane will always be z_center-focal_length away
                    // from the focal point

                    double percent_x = (j + 0.5) / image_width;
                    double percent_y = (i + 0.5) / image_height;

                    double screen_x =
                        center.vec[0] + (2.0 * percent_x - 1.0) * aspect_ratio;
                    double screen_y = center.vec[1] - (2.0 * percent_y - 1.0);
                    double screen_z = center.vec[2] - focal_length;

                    vec3 screen_point = vec3{screen_x, screen_y, screen_z};
                    vec3 dir = unit_vector(screen_point - center);

                    ray r = ray(center, dir);

                    // NOTE: call color() here which will take in the ray
                    // and return the color that we are looking for.
                }
            }
            break;
        }
        }
    }
    inline void color() { return; }
};
#endif
