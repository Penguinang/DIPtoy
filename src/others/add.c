/**
 * 图像叠加
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
#define INPUT1 "pics/noised_sun.png"
#define INPUT2 "pics/shi.jpeg"
#define OUTPUT "pics/add.png"
#define S_N 2
    char *input[S_N] = {INPUT1, INPUT2};
    unsigned int x[S_N], y[S_N], channels[S_N];
    unsigned char *source[S_N] = {};
    for (int i = 0; i < S_N; ++i)
        source[i] = stbi_load(input[i], x + i, y + i, channels + i, 0);

    int x_g = x[0], y_g = y[0], channels_g = channels[0];
    for (int i = 1; i < S_N; ++i) {
        x_g = x[i] < x_g ? x[i] : x_g;
        y_g = y[i] < y_g ? y[i] : y_g;
        channels_g = channels[i] < channels_g ? channels[i] : channels_g;
    }
    unsigned char *dest = malloc(sizeof(unsigned char) * x_g * y_g * channels_g);
    memset(dest, 0, sizeof(unsigned char) * x_g * y_g * channels_g);

    for (int ix = 0; ix < x_g; ++ix)
        for (int iy = 0; iy < y_g; ++iy) {
            unsigned char *point = dest + (iy * x_g + ix) * channels_g;
            for (int ic = 0; ic < channels_g; ++ic) {
                // point[ic] /= S_N;
                // for(int is = 0; is < S_N; ++ is){
                // point[ic] += source[is][(iy * x[is] + ix) * channels[is] + ic] / S_N;
                // point[ic] += source[is][(iy * x[is] + ix) * channels[is] + ic];
                // point[ic] -= source[is][(iy * x[is] + ix) * channels[is] + ic];
                // }

                // point[ic] = source[0][(iy * x[0] + ix) * channels[0] + ic] * 1.0 * source[1][(iy
                // * x[1] + ix) * channels[1] + ic] / 255;
            }
        }

    stbi_write_png(OUTPUT, x_g, y_g, channels_g, dest, 0);
    free(dest);
    return 0;
}
