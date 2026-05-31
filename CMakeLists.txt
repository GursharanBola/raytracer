cmake_minimum_required(VERSION 3.10)
project(raytracer)

include_directories(include external)

set(SOURCES
    src/camera.cpp
    src/glass.cpp
    src/img.cpp
    src/lambertian.cpp
    src/light.cpp
    src/main.cpp
    src/metal.cpp
    src/sphere.cpp
)

add_executable(raytracer ${SOURCES})
