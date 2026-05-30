#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {

    // We need to find a way to add and remove objects from the list

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
        double closest_dist = ray_tmax;
        bool did_hit = false;

        // NOTE: This is only safe if rec is written to only on true statements!
        // For any future shapes we impliment we need it so that we only write
        // to rec IFF we have a valid intersection.

        for (const auto &object : this->objects) {
            if (object->hit(r, ray_tmin, closest_dist, rec)) {
                did_hit = true;
                closest_dist = rec.t;
            }
        }
        return did_hit;
    }
};

#endif
