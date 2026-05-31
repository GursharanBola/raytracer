#include "material.h"
#include "vec3.h"

// NOTE: Lights should be sufficiently bright to work on the system
class light : public material {
  public:
    light(vec3 material_color) { color = material_color; }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction, bool is_entering) const override {
        return vec3{0, 0, 0}; // don't bounce
    }
    bool is_light() const override { return true; }
};
