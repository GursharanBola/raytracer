#ifndef GLASS_H
#define GLASS_H

#include "material.h"
#include "vec3.h"

// TODO: Finish the glass
class glass_surface : public material {
  public:
    double refrac_index;
    glass_surface(vec3 material_color, double refractive_index) {
        color = material_color;
        refrac_index = refractive_index;
    }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction) const override {

        return vec3{0, 0, 0};
    }
};
#endif
