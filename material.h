#ifndef MATERIAL
#define MATERIAL

#include "ray.h"
#include "vec3.h"

class hittable_list;
class hit_record;

class material {
  public:
    virtual ~material() = default;
    vec3 color{0, 0, 0};
    // matte surfaces need to know the hit_location, whereas metals and glass
    // need to now the ray_direction
    virtual vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                        const hittable_list &world,
                        vec3 ray_direction) const = 0;
};

#endif
