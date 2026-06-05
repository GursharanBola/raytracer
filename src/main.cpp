#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "img.h"
#include "material.h"
#include "vec3.h"
#include <filesystem>
#include <memory>

int main() {
    hittable_list world;

    world.add(std::make_shared<sphere>(
        vec3{0, -100.5, -1}, 100,
        std::make_shared<lambertian>(vec3{0.5, 0.5, 0.5})));

    world.add(std::make_shared<sphere>(
        vec3{-1.2, 0, -1.0}, 0.5,
        std::make_shared<lambertian>(vec3{0.7, 0.3, 0.3})));
    world.add(std::make_shared<sphere>(
        vec3{0.0, 0, -2.0}, 0.5,
        std::make_shared<lambertian>(vec3{0.3, 0.7, 0.3})));
    world.add(std::make_shared<sphere>(
        vec3{1.2, 0, -3.0}, 0.5,
        std::make_shared<lambertian>(vec3{0.3, 0.3, 0.7})));

    vec3 camera_center = vec3{0, 0, 0};
    vec3 look_at = vec3{0, 0, -2};
    vec3 cam_w = unit_vector(camera_center - look_at);
    vec3 cam_u = unit_vector(cross(vec3{0, 1, 0}, cam_w));
    vec3 cam_v = cross(cam_w, cam_u);

    double f_length = 2.0;
    double focus_dist = (vec3{0, 0, -2} - camera_center).length();

    camera cam = camera(camera_center, camera::CAMERA_FLAT, focus_dist,
                        std::numeric_limits<double>::max(), 0.001, f_length,
                        cam_u, cam_v, cam_w);

    img image;
    int image_width = 800;
    int image_height = 600;

    int color_channels = 3;
    int stride = color_channels * image_width;

    cam.render(world, image_width, image_height, image);

    namespace fs = std::filesystem;
    if (!fs::exists("out"))
        fs::create_directory("out");
    image.draw_png("out/render.png", image_width, image_height, color_channels,
                   image.image_buffer.data(), stride);
}
