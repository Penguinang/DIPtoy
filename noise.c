/**
 * 躁化
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// An open source image load and write library
// https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char const *argv[]) {
    if(argc < 3){
        printf("Wrong argument\n");
        return -1;
    }
// #define INPUT "../res/me.jpg"
// #define OUTPUT "noised.png"
#define INPUT argv[1]
#define OUTPUT argv[2]
    unsigned int x, y, channels;
    unsigned char *source = stbi_load(INPUT, &x, &y, &channels, 0);

    srand(time(NULL));
    for (size_t i = 0; i < x; i++) {
        for (size_t j = 0; j < y; j++) {
            unsigned char *point = source + (j * x + i) * channels;
            int r = rand();
            if(r % 100 < 1)
                for(int c = 0; c < channels; ++ c)
                    point[c] = 1.0 * r / RAND_MAX * 255;
            srand(r);
        }
    }

    stbi_write_png(OUTPUT, x, y, channels, source, 0);
    return 0;
}
