#ifndef IMG
#define IMG

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

class img {
public:
        static constexpr int length = 800;
        static constexpr int width = 600;

        std::array<uint8_t, length * width * 3> image_buffer;


};
#endif IMG
