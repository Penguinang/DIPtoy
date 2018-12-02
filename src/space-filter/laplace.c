/**
 * 拉普拉斯算子
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An open source image load and write library
// https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char const *argv[]) {
    if(argc <3){
        printf("Wrong args\n");
        return -1;
    }
#define INPUT argv[1]
#define OUTPUT argv[2]
#define K_SIZE 3
    int convolution_kernel[K_SIZE][K_SIZE] = {
        {1,  1, 1},
        {1, -8, 1},
        {1,  1, 1}
    };

    unsigned char *source, *dest;
    int x, y, channels;
    source = stbi_load(INPUT, &x, &y, &channels, 0);
    dest = malloc(sizeof(unsigned char) * x * y * channels);
    memcpy(dest, source, sizeof(unsigned char) * x * y * channels);

    unsigned char cvt_value[4] = {};
    for(int iy = K_SIZE/2; iy < y - K_SIZE/2; ++ iy)
        for(int ix = K_SIZE/2; ix < x - K_SIZE/2; ++ ix){
            for(int m = 0; m < K_SIZE; ++ m)
                for(int n = 0; n < K_SIZE; ++ n){
                    unsigned char *s_point = source + ((iy + m - K_SIZE/2) * x + ix + n - K_SIZE/2) * channels;
                    for(int ic = 0; ic < channels; ++ ic){
                        cvt_value[ic] += convolution_kernel[m][n] * s_point[ic];
                    }
                }
            unsigned char *d_point = dest + (iy * x + ix) * channels;;
            for(int ic = 0; ic < channels; ++ ic){
                d_point[ic] -= cvt_value[ic];
                cvt_value[ic] = 0;
            }
        }

    stbi_write_png(OUTPUT, x, y, channels, dest, 0);
    free(dest);
    return 0;
}
