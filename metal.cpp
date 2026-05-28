#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "vec3.h"

class metal_surface : public material {
  public:
    double fuzziness;
    metal_surface(vec3 material_color, double fuzz) {
        color = material_color;
        fuzziness = (fuzz < 0.0) ? 0.0 : ((fuzz > 1.0) ? 1.0 : fuzz);
    }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction) const override {
        vec3 res;
        res = ray_direction - 2 * (dot(ray_direction, normal)) * normal;

        double radius = fuzziness;
        vec3 fuzz = random_vec3(-radius, radius);

        while (dot(fuzz, fuzz) > radius * radius) {
            fuzz = random_vec3(-radius, radius);
        }
        res = res + hit_location + fuzz;           // this is a spatial vector.
        if (dot(res - hit_location, normal) > 0) { // this is direction vector.
            return res;
        } else {
            return vec3{0, 0, 0};
        }
    }
};
#endif
