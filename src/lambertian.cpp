#include "material.h"
#include "vec3.h"

vec3 lambertian::bounce(const vec3 &hit_location, const vec3 &normal,
                        const vec3 ray_direction, bool is_entering) const {
    int radius = 1;
    vec3 res = random_vec3(-radius, radius);

    while (dot(res, res) > radius * radius) {
        res = random_vec3(-radius, radius);
    }

    vec3 bounce_direction = normal + res;

    double s = 1e-8;
    if ((std::fabs(bounce_direction.vec[0]) < s) &&
        (std::fabs(bounce_direction.vec[1]) < s) &&
        (std::fabs(bounce_direction.vec[2]) < s)) {
        bounce_direction = normal;
    }

    return bounce_direction / bounce_direction.length();
}
