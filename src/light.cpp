#include "material.h"
#include "vec3.h"

// NOTE: Lights should be sufficiently bright to work on the system
vec3 light::bounce(const vec3 &hit_location, const vec3 &normal,
                   const vec3 &ray_direction, bool is_entering) const {
    return vec3{0, 0, 0}; // don't bounce
}
