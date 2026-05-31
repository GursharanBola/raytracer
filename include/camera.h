#ifndef CAMERA_H
#define CAMERA_H
#include "hittable_list.h"
#include "material.h"
#include "vec3.h"
#include <limits>

class camera {
  public:
    virtual ~camera() = default;
    enum CameraType {
        CAMERA_FLAT = 0,
        CAMERA_SPHERICAL = 1,
        CAMERA_FISHEYE = 2
    };

    camera(const vec3 &camera_center, CameraType type, double f_dist,
           double tray_max = std::numeric_limits<double>::max(),
           double tray_min = 0.001, double f_length = 1.0,
           vec3 u_axis = vec3(1, 0, 0), vec3 v_axis = vec3(0, 1, 0),
           vec3 w_axis = vec3(0, 0, 1))
        : center(camera_center), cam_u(u_axis), cam_v(v_axis), cam_w(w_axis),
          camera_type(type), focal_length(f_length), focal_dist(f_dist),
          t_max{tray_max}, t_min{tray_min} {}

    void render(const hittable_list &world, int image_width, int image_height);

  private:
    vec3 center;
    vec3 cam_u; // right
    vec3 cam_v; // up
    vec3 cam_w; // backward
    CameraType camera_type;
    double focal_length;
    double pi = 3.1415926535897932;
    double focal_dist;
    double aperture_radius = 0.5;
    double t_max;
    double t_min;
    int num_samples = 20; // number of antialiasing samples.
    int depth = 10;
    vec3 average_pixel_angular(int i, int j, double delta_theta,
                               double delta_phi,
                               const hittable_list &world) const;

    vec3 average_pixel_linear(int i, int j, const hittable_list &world,
                              int image_width, int image_height) const;

    vec3 color(const ray &r, const hittable_list &world, int depth) const;
    vec3 gamma_correct(const vec3 &color) const;
};
#endif
