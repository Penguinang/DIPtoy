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

unsigned char transform(unsigned char);

int main(int argc, char const *argv[]) {
    if(argc < 3){
        printf("Wrong args\n");
        return -1;
    }

    int x, y, channels;
    unsigned char *source, *dest;
    
    source = stbi_load(argv[1], &x, &y, &channels, 0);
    dest = malloc(sizeof(unsigned char) * x * y * channels);
    memcpy(dest, source, sizeof(unsigned char) * x * y * channels);

    for(int iy = 0; iy < y; ++ iy){
        for(int ix = 0; ix < x; ++ ix){
            for(int c = 0; c < channels; ++ c){
                dest[(iy * x + ix) * channels + c] = transform(source[(iy * x + ix) * channels + c]);
            }
        }
    }

    stbi_write_png(argv[2], x, y, channels, dest, 0);
    return 0;
}
