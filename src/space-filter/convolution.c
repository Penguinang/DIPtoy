/**
 * 通用卷积操作
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// An open source image load and write library
// https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Generalization version to multiply two vector
// *result must be initialized to 0 before this function
void vec_mul(const void *a, const void *b, void *result, size_t count, size_t size,
              void (*mul)(const void *, const void *, void *),
              void (*add)(const void *, const void *, void *)) {
    void *product = malloc(size);
    memcpy(product, result, size);
    for (size_t i = 0; i < count; ++i) {
        mul(a + i * size, b + i * size, product);
        add(product, result, result);
    }

    free(product);
}
// Generalization version to sum a vector
// *result must be initialized to 0 before this function
void sum(const void *a, void *result, size_t count, size_t size, void (*add)(const void *, const void*, void*)){
    for(size_t i = 0; i < count; ++ i){
        add(result, a + i * size, result);
    }
}
// Specialization for int
void mul(const void *a, const void *b, void *p) { *(int *)p = *(int *)a * *(int *)b; }
// Specialization for int
void add(const void *a, const void *b, void *s) { *(int *)s = *(int *)a + *(int *)b; }
// Specialization for double
void d_add(const void *a, const void *b, void *s) { *(double *)s = *(double *)a + *(double *)b; }

int main(int argc, char const *argv[]) {
    if(argc < 3){
        printf("Wrong args\n");
        return -1;
    }
#define INPUT argv[1]
#define OUTPUT argv[2]
 /*#define INPUT "gray-scale.png"*/
//  #define INPUT "noised.png"
// #define INPUT "../res/me.jpg"
/*#define INPUT "ppt.png"*/
// #define OUTPUT "convolution.png"

    unsigned int x, y, channels;
    unsigned char *source = stbi_load(INPUT, &x, &y, &channels, 0);
    unsigned char *dest = malloc(sizeof(unsigned char) * x * y * channels);
    memcpy(dest, source, sizeof(unsigned char) * x * y * channels);
#define CONVOLUTION_WIDTH 3
#define DOUBLE_DIRECTION 0
#define NORMALIZATION 0
    double convolution_kernel[CONVOLUTION_WIDTH][CONVOLUTION_WIDTH] = {
        // average filter
        // {1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH}, //
        // {1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH}, //
        // {1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH}, //
        // {1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH}, //
        // {1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH, 1.0/CONVOLUTION_WIDTH/CONVOLUTION_WIDTH}  //
        
        // sobel
        // {-1, 0, 1},
        // {-2, 0, 2},
        // {-1, 0, 1}

        // Laplace
        {1, 1, 1},
        {1, -8, 1},
        {1, 1, 1}

        // prewitt
        // {-1, 0, 1},
        // {-1, 0, 1},
        // {-1, 0, 1}

        // simple gauss
        // {3,  3,  3,  3, 3}, //
        // {3, -2, -2, -2, 3}, //
        // {3, -2, 20, -2, 3}, //
        // {3, -2, -2, -2, 3}, //
        // {3,  3,  3,  3, 3}  //        
    };
#if NORMALIZATION
    double con_sum = 0;
    sum(convolution_kernel, &con_sum, CONVOLUTION_WIDTH * CONVOLUTION_WIDTH, sizeof(**convolution_kernel), d_add);
    for(int i = 0; i < CONVOLUTION_WIDTH; ++ i)
        for(int j = 0; j < CONVOLUTION_WIDTH; ++ j)
            convolution_kernel[i][j] /= con_sum;
#endif

#define MAX_CHANNELS 4
    int convolution_sum[MAX_CHANNELS] = {};
    
#if DOUBLE_DIRECTION
    int convolution_kernel2[CONVOLUTION_WIDTH][CONVOLUTION_WIDTH] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    int convolution_sum2[MAX_CHANNELS] = {};
#endif

    for (int i = CONVOLUTION_WIDTH/2; i < x-CONVOLUTION_WIDTH/2; ++i)
        for (int j = CONVOLUTION_WIDTH/2; j < y-CONVOLUTION_WIDTH/2; ++j) {
            for (int m = 0; m < CONVOLUTION_WIDTH; ++m)
                for (int n = 0; n < CONVOLUTION_WIDTH; ++n) {
                    unsigned char *c_point = source + ((j+m-CONVOLUTION_WIDTH/2) * x + i + n-CONVOLUTION_WIDTH/2) * channels;
                    for (int c = 0; c < channels; ++c){
                        convolution_sum[c] += c_point[c] * convolution_kernel[m][n];
                        #if DOUBLE_DIRECTION 
                        convolution_sum2[c] += c_point[c] * convolution_kernel2[m][n];
                        #endif
                    }
                }

            unsigned char *point = dest + (j * x + i) * channels;
            for(int c = 0; c < channels; ++ c){
                #if !DOUBLE_DIRECTION
                point[c] = (convolution_sum[c] > 0 ? convolution_sum[c] : -convolution_sum[c]);
                #else
                point[c] = sqrt(pow(convolution_sum[c], 2) + pow(convolution_sum2[c], 2));
                convolution_sum2[c] = 0;
                #endif
                convolution_sum[c] = 0;
            }
        }

    stbi_write_png(OUTPUT, x, y, channels, dest, 0);
    return 0;
}
