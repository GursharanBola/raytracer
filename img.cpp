#ifndef IMG_H
#define IMG_H

#include "stb_image.h"
#include "stb_image_write.h"
#include <string>

#include <array>
#include <cstdint>

// TODO: Create some main function so that we can use these header files.
// TODO: Need to create the rendering engine for this using a library.

class img {
  public:
    static constexpr int width = 800;
    static constexpr int height = 600;
    static constexpr int channels = 3;

    std::array<uint8_t, width * height * channels> image_buffer;

    void draw(std::string filename, int width, int height, int channels,
              void *data, int stride) {}
};

#endif
