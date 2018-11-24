/**
 * 中值滤波
 */

#include <stdio.h>

// An open source image load and write library
// https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int lt(const void *a, const void *b) { return *(unsigned int *)a < *(unsigned int *)b; }

int main(int argc, char const *argv[]) {
    if(argc < 3){
        printf("Wrong argument\n");
        return -1;
    }
// #define INPUT "noised.png"
// #define OUTPUT "median.png"
#define INPUT argv[1]
#define OUTPUT argv[2]

    unsigned int x, y, channels;
    unsigned char *source = stbi_load(INPUT, &x, &y, &channels, 0);

#define CONVOLUTION_WIDTH 5
#define MAX_CHANNELS 4
    unsigned int convolution_kernels[MAX_CHANNELS][CONVOLUTION_WIDTH][CONVOLUTION_WIDTH] = {};

    for (size_t i = CONVOLUTION_WIDTH / 2 + 1; i <= x - CONVOLUTION_WIDTH / 2; ++i) {
        for (size_t j = CONVOLUTION_WIDTH / 2 + 1; j <= y - CONVOLUTION_WIDTH / 2; ++j) {
            unsigned char *point = source + (j * x + i) * channels;
            for (int m = -CONVOLUTION_WIDTH / 2; m <= CONVOLUTION_WIDTH / 2; ++m) {
                for (int n = -CONVOLUTION_WIDTH / 2; n <= CONVOLUTION_WIDTH / 2; ++n) {
                    unsigned char *c_point = source + ((j + m) * x + (i + n)) * channels;
                    for (int l = 0; l < channels; l++) {
                        convolution_kernels[l][m + CONVOLUTION_WIDTH / 2]
                                           [n + CONVOLUTION_WIDTH / 2] = c_point[l];
                    }
                }
            }
            for(int l = 0; l < channels; ++ l){
                qsort(convolution_kernels + l, CONVOLUTION_WIDTH * CONVOLUTION_WIDTH, sizeof(unsigned int), lt);
                point[l] = convolution_kernels[l][0][CONVOLUTION_WIDTH * CONVOLUTION_WIDTH / 2];
            }
        }
    }
    stbi_write_png(OUTPUT, x, y, channels, source, 0);
    return 0;
}
