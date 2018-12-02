#include <math.h>

#define C 10
unsigned char transform(unsigned char s){
    return C * log(s+1);
}