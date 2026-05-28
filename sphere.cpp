#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    sphere(const vec3 &center, double radius)
        : center(center), radius(std::fmax(0, radius)) {}

    // TODO: Return material of the sphere that we have hit.
    bool hit(const ray &r, double t_min, double t_max,
             hit_record &rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto deter = h * h - a * c;
        auto sqrt_deter = std::sqrt(deter);

        // Check if an intersection point exists
        if (deter < 0) {
            return false;
        }

        // check if we are in range and if so which ones are valid checking
        // closest first
        auto root = (h - sqrt_deter) / a;
        if (root <= t_min || t_max <= root) {
            root = (h + sqrt_deter) / a;
            if (root <= t_min || t_max <= root)
                return false;
        }

        // Save the item to the drawer which we will read from later.
        rec.t = root;
        rec.point = r.at(rec.t);
        rec.normal = (rec.point - center) / radius;
        vec3 outward_normal = (rec.point - center) / radius;
        rec.set_face_normal(r, outward_normal);
        return true;
    }
    vec3 get_center() { return center; }
    double get_radius() { return radius; }

  private:
    vec3 center;
    double radius;
};
