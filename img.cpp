#ifndef IMG_H
#define IMG_H

#include "stb_image.h"
#include "stb_image_write.h"
#include "vec3.h"
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

    std::array<uint8_t, height * width * channels> image_buffer;
    img() {};

    int draw_png(std::string filename, int width, int height, int channels,
                 void *data, int stride) {
        int png_truth = stbi_write_png(filename.c_str(), width, height,
                                       channels, data, stride);
        return png_truth;
    }

    vec3 get_color(const int x, const int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return vec3(0, 0, 0);
        }

        int row_size = 3 * width;
        int index = (3 * x) + (y * row_size);

        return vec3(image_buffer[index] / 255.0,
                    image_buffer[index + 1] / 255.0,
                    image_buffer[index + 2] / 255.0);
    }
};

#endif
