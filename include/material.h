#ifndef MATERIAL
#define MATERIAL

#include "vec3.h"

class hittable_list;
class hit_record;

class material {
  public:
    virtual ~material() = default;
    vec3 color{0, 0, 0};
    // matte surfaces need to know the hit_location, whereas metals and glass
    // need to now the ray_direction. Glass also needs to know is_entering.
    virtual vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                        vec3 ray_direction, bool is_entering) const = 0;
    virtual bool is_light() const { return false; }
};

class lambertian : public material {
  public:
    lambertian(vec3 material_color) { color = material_color };
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction, bool is_entering) const override;
};

class glass : public material {
  public:
    double refrac_index;
    glass(vec3 material_color, double refractive_index) {
        color = material_color;
        refrac_index = refractive_index;
    }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction, bool is_entering) const override;
};

class metal : public material {
  public:
    double fuzziness;
    metal(vec3 material_color, double fuzz) {
        color = material_color;
        fuzziness = (fuzz < 0.0) ? 0.0 : ((fuzz > 1.0) ? 1.0 : fuzz);
    }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction, bool is_entering) const override;
};

class light : public material {
  public:
    light(vec3 material_color) { color = material_color; }
    vec3 bounce(const vec3 &hit_location, const vec3 &normal,
                const vec3 ray_direction, bool is_entering) const override;
    bool is_light() const override { return true; }
};
#endif
