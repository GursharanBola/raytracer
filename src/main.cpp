#include "camera.h"
#include "hittable_list.h"
#include "img.h"
#include "ray.h"
#include "sphere.cpp"
#include "vec3.h"

int main() {
    hittable_list world = hittable_list();
    sphere shiny_ball = sphere();
    world.add(shiny_ball);
}
