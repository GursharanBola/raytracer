#ifndef LAMBERTIAN_SURFACE_H
#define LAMBERTIAN_SURFACE_H

#include "material.h"
#include "vec3.h"

class lambertian_surface : public material {
  public:
    // It must be that we send in the color before we make the item.
    lambertian_surface(vec3 material_color, double absorbed) {
        color = material_color;
    }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction) const override {
        int radius = 1;
        vec3 res = random_vec3(-radius, radius);

        while (dot(res, res) > radius || dot(normal, res) < 0) {
            res = random_vec3(-radius, radius);
        }

        vec3 bounce_direction = normal + res;

        return bounce_direction / bounce_direction.length();
    }
};
#endif
