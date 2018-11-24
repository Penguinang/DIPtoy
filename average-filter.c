/**
 * 均值滤波
 */
#include <stdio.h>

// An open source image load and write library
// https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char const *argv[]) {
    if(argc < 3){
        printf("Wrong args\n");
        return -1;
    }
#define INPUT argv[1]
#define OUTPUT argv[2]
// #define INPUT "noised.png"
// #define OUTPUT "average.png"
    unsigned int x, y, channels;
    unsigned char *source = stbi_load(INPUT, &x, &y, &channels, 0);
    unsigned char *dest = malloc(sizeof(unsigned char) * x * y * channels);
    memcpy(dest, source, sizeof(unsigned char) * x * y * channels);

#define CONVOLUTION_WIDTH 5

    for (size_t i = CONVOLUTION_WIDTH / 2 + 1; i < x - CONVOLUTION_WIDTH / 2 - 1; i++) {
        for (size_t j = CONVOLUTION_WIDTH / 2 + 1; j < y - CONVOLUTION_WIDTH / 2 - 1; j++) {
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0;
            for (int m = -CONVOLUTION_WIDTH / 2; m < CONVOLUTION_WIDTH/2+1; m++) {
                for (int n = -CONVOLUTION_WIDTH / 2; n < CONVOLUTION_WIDTH/2+1; n++) {
                    unsigned char *c_point = source + ((j+m) * x + (i+n)) * channels;
                    sum_r += c_point[0];
                    sum_g += c_point[1];
                    sum_b += c_point[2];
                }
            }
            unsigned char *point = dest + (j * x + i) * channels;
            point[0] = sum_r/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH;
            point[1] = sum_g/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH;
            point[2] = sum_b/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH;
        }
    }

    stbi_write_png(OUTPUT, x, y, channels, dest, 0);
    return 0;
}
