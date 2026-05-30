#ifndef GLASS_H
#define GLASS_H

#include "material.h"
#include "vec3.h"

class glass_surface : public material {
  public:
    double refrac_index;
    glass_surface(vec3 material_color, double refractive_index) {
        color = material_color;
        refrac_index = refractive_index;
    }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction, bool is_entering) const override {

        double cos_theta = std::fmin(-dot(ray_direction, normal), 1.0);
        double eta_ratio = is_entering ? (1.0 / refrac_index) : refrac_index;
        double sin_squared = 1 - cos_theta * cos_theta;
        double feas_check = eta_ratio * eta_ratio * sin_squared;

        if (feas_check > 1) {
            return ray_direction - 2 * (dot(ray_direction, normal)) * normal;
        } else {
            vec3 R_perp = eta_ratio * (ray_direction +
                                       (-dot(ray_direction, normal) * normal));
            vec3 R_parallel =
                -1 * std::sqrt(1 - R_perp.length() * R_perp.length()) * normal;
            return R_perp + R_parallel;
        }
    }
};
#endif
