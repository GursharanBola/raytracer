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
        vec3{0, -100.5, -3}, 100,
        std::make_shared<lambertian>(vec3{0.8, 0.8, 0.0})));

    world.add(std::make_shared<sphere>(
        vec3{-1.0, 1.5, 1.0}, 0.5,
        std::make_shared<metal>(vec3{0.1, 0.3, 0.9}, 0.2))); // Blue

    world.add(std::make_shared<sphere>(
        vec3{3.0, 1.5, 2.0}, 1.0,
        std::make_shared<metal>(vec3{0.8, 0.8, 0.8}, 0.1))); // White

    world.add(std::make_shared<sphere>(
        vec3{2.8, 1.5, -0.8}, 1.0,
        std::make_shared<lambertian>(vec3{0.2, 0.9, 0.2}))); // Green

    world.add(std::make_shared<sphere>(
        vec3{0, 0.5, -6}, 1.0,
        std::make_shared<lambertian>(vec3{1.0, 0.0, 1.0})));

    world.add(std::make_shared<sphere>(
        vec3{0.0, 1.2, 1.3}, 0.4,
        std::make_shared<metal>(vec3{0.2, 0.9, 0.2}, 0.4))); // Green

    // split
    world.add(std::make_shared<sphere>(
        vec3{0.0, 1.5, 2.8}, 0.5,
        std::make_shared<metal>(vec3{0.9, 0.4, 0.1}, 0.0))); // Blue

    vec3 camera_center = vec3{0, 1.5, 2};
    vec3 look_at = vec3{0, 0.5, -3}; // Orientation reference
    vec3 cam_w = unit_vector(camera_center - look_at);
    vec3 cam_u = unit_vector(cross(vec3{0, 1, 0}, cam_w));
    vec3 cam_v = cross(cam_w, cam_u);
    double focal_dist = (camera_center - look_at).length();

    camera cam = camera(camera_center, camera::CAMERA_FISHEYE, focal_dist,
                        std::numeric_limits<double>::max(), 0.001, 1.0, cam_u,
                        cam_v, cam_w);

    img image;
    int image_width = 600;
    int image_height = 600;
    int color_channels = 3;
    int stride = image_width * color_channels;

    cam.render(world, image_width, image_height, image);

    namespace fs = std::filesystem;
    if (!fs::exists("out"))
        fs::create_directory("out");
    image.draw_png("out/render.png", image_width, image_height, color_channels,
                   image.image_buffer.data(), stride);
}
