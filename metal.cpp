#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "vec3.h"

// TODO: Finish the metal surfaces
class metal_surface : public material {
  public:
    double fuzziness;
    metal_surface(vec3 material_color, double fuzz) {
        color = material_color;
        fuzziness = fuzz;
    }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const hittable_list &world,
                const vec3 ray_direction) const override {

        return vec3{0, 0, 0};
    }
};
#endif
