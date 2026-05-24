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
        vec3 curr_hor_cir =
            vec3{0, focal_length, 0}; // start at the top of the sphere
        double delta_theta = pi / (image_height - 1);
        double delta_phi = pi / (image_width - 1);
        int t_max = 100;
        int t_min = 0;
        switch (camera_type) {
        case CAMERA_SPHERICAL:
            for (int i = 0; i < image_height; i++) {

                double x_k = curr_hor_cir.vec[0];
                double y_k = curr_hor_cir.vec[1];
                double z_k = curr_hor_cir.vec[2];
                double y_next =
                    y_k * std::cos(delta_theta) - z_k * std::sin(delta_theta);
                double z_next =
                    y_k * std::sin(delta_theta) + z_k * std::cos(delta_theta);

                curr_hor_cir = vec3{x_k, y_next, z_next};

                for (int j = 0; i < image_width; j++) {
                    double x_k = curr_hor_cir.vec[0];
                    double y_k = curr_hor_cir.vec[1];
                    double z_k = curr_hor_cir.vec[2];

                    double x_next =
                        x_k * std::cos(delta_phi) - z_k * std::sin(delta_phi);
                    double z_next =
                        x_k * std::sin(delta_phi) + z_k * std::cos(delta_phi);

                    curr_hor_cir = vec3{x_next, y_k, z_next};

                    ray r = ray(center, curr_hor_cir - center);

                    // NOTE: call color() here which will take in the ray
                    // and return the color that we are looking for.
                }
            }
            break;
        case CAMERA_FLAT:
            break;
        }
    }
};

inline void color() { return; }
#endif
