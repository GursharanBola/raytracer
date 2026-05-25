#ifndef IMG_H
#define IMG_H

#include "stb_image.h"
#include "stb_image_write.h"
#include "vec3.h"
#include <string>

#include <array>
#include <cstdint>

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

        int index = (3 * x) + 3 * width * y;

        return vec3(image_buffer[index] / 255.0,
                    image_buffer[index + 1] / 255.0,
                    image_buffer[index + 2] / 255.0);
    }
    int set_color(const int x, const int y, vec3 color) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return 0; // We failed
        }

        int index = 3 * x + 3 * width * y;

        image_buffer[index] = static_cast<int>(255.999 * color.vec[0]);
        image_buffer[index + 1] = static_cast<int>(255.999 * color.vec[1]);
        image_buffer[index + 2] = static_cast<int>(255.999 * color.vec[2]);

        return 1;
    }
};

#endif
