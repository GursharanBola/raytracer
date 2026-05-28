#ifndef LAMBERTIAN_SURFACE_H
#define LAMBERTIAN_SURFACE_H

#include "material.h"
#include "vec3.h"

class lambertian_surface : public material {
  public:
    lambertian_surface();
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const hittable_list &world) const override {
        vec3 res = random_vec3();

        while () {
        }

        return res;
    }
};

#endif
