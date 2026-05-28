#ifndef LAMBERTIAN_SURFACE_H
#define LAMBERTIAN_SURFACE_H

#include "material.h"

class lambertian_surface : public material {
  public:
    lambertian_surface();
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const hittable_list &world) const override {
        return vec3{0, 0, 0};
    }
};

#endif
