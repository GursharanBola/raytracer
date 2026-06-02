#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.cpp"
#include "vec3.h"
#include <memory>

int main() {
    hittable_list world = hittable_list();
    vec3 center = vec3{0, 0, 0}; // make enter at the origin
    double radius = 1.0;
    vec3 material_color = vec3{1, 1, 1}; // no attenutation
    double fuzz = 0;
    std::shared_ptr<material> mat =
        std::make_shared<metal>(material_color, fuzz);

    std::shared_ptr<hittable> shiny_ball =
        std::make_shared<sphere>(center, radius, mat);

    // TODO: Make the camera and orient the scene correctly.

    world.add(shiny_ball);
}
