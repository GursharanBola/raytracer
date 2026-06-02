#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include <memory>

/*
** We will be using the vec3 class for both points AND vectors AND colors
*/
class material;

class hit_record {
  public:
    vec3 point;
    vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<material> mat;

    void set_face_normal(const ray &r, vec3 outward_normal) {
        if (dot(r.direction(), outward_normal) > 0.0) {
            normal = -outward_normal;
            front_face = false; // the ray is on the inside
        } else {
            normal = outward_normal;
            front_face = true; // the ray is on the outside
        }
    }
};

class hittable {
  public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, double ray_tmin, double ray_tmax,
                     hit_record &rec) const = 0;
};

class sphere : public hittable {
  public:
    sphere(const vec3 &center, double radius, std::shared_ptr<material> mat)
        : center(center), radius(std::fmax(0, radius)), mat(mat) {};

    bool hit(const ray &r, double t_min, double t_max,
             hit_record &rec) const override;
    inline vec3 get_center() const { return center; }
    inline double get_radius() const { return radius; }

  private:
    vec3 center;
    double radius;
    std::shared_ptr<material> mat;
};
#endif
