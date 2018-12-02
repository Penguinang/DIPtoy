#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An open source image load and write library
// https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char const *argv[])
{
    unsigned char out[100][100] = {};

    for(int i = 0; i < 100; ++ i)
        for(int j = 0; j < 100; ++ j){
            out[i][j] = -i * 255 /100;
        }

    stbi_write_png("test.png", 100, 100, 1, out, 0);
    return 0;
}
