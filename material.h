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
    virtual vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                        const hittable_list &world) const = 0;
};

#endif
