#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include <iostream>
#include <vector>

class ray {
public:
  ray() {}

  ray(const vec3 &origin, const vec3 &direction)
      : orig(origin), dir(direction) {}

  vec3 origin() const { return orig; }
  vec3 direction() const { return dir; }
  vec3 at(const double t) const { return orig + dir * t; }

private:
  vec3 orig;
  vec3 dir;
};
#endif
