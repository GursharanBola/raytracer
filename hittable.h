#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include <iostream>
#include <vector>

/*
** We will be using the vec3 class for both points AND vectors
*/
class hit_record {
public:
  vec3 point;
  vec3 normal;
  double t;
  bool front_face;

  void set_face_normal(const ray &r, vec3 outward_normal) {
    if (dot(r.direction(), outward_normal) > 0.0) {
      normal = -outward_normal;
      front_face = false; // the ray is on the inside
    } else {
      normal = outward_normal;
      front_face = true; // the ray is on the outside
    }
  }
};

// This was largely inspired by RTIOW
class hittable {
public:
  virtual ~hittable() = default;
  virtual bool hit(const ray &r, double ray_tmin, double ray_tmax,
                   hit_record &rec) const = 0;
};
#endif
