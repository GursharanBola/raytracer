#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include "hittable.h"
#include "ray.h"
#include <algorithm>
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> item) { add(item); }

    void remove(shared_ptr<hittable> item) {
        this->objects.erase(std::remove(objects.begin(), objects.end(), item),
                            objects.end());
    }

    void add(shared_ptr<hittable> item) {
        if (item == nullptr) {
            return;
        }
        this->objects.push_back(item);
    }

    /* only return the closest since ray cannot pass through other items */
    bool hit(const ray &r, double ray_tmin, double ray_tmax,
             hit_record &rec) const override {

        // TODO: Bound object to quickly determine if we have to actually render
        // some of the objects.
        hit_record temp_rec;
        double closest_dist = ray_tmax;
        bool did_hit = false;

        for (const auto &object : this->objects) {
            if (object->hit(r, ray_tmin, closest_dist, rec)) {
                did_hit = true;
                closest_dist = rec.t;

                rec = temp_rec;
            }
        }
        return did_hit;
    }
};

#endif
