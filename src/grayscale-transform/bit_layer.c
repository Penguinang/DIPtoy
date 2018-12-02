#define L 4

unsigned char transform(unsigned char s){
    return s & (1 << L);
}