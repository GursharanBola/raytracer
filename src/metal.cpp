#include "material.h"
#include "vec3.h"

vec3 metal::bounce(const vec3 &hit_location, const vec3 &normal,
                   const vec3 &ray_direction, bool is_entering) const {
    vec3 res;
    res = ray_direction - 2 * (dot(ray_direction, normal)) * normal;

    double radius = fuzziness;
    vec3 fuzz = random_vec3(-radius, radius);

    while (dot(fuzz, fuzz) > radius * radius) {
        fuzz = random_vec3(-radius, radius);
    }
    res = res + fuzz;
    if (dot(res, normal) > 0) {
        return res / res.length();
    } else {
        return vec3{0, 0, 0};
    }
}
