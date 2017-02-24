#include <math.h>

#define CHANNEL 4
#define BLUR 20

int * sizes;
int * pixel_val;

void gauss_blur();
void box_gauss(double sigma, int n);
void box_blur(png_bytep* scl, png_bytep* tcl, int r);
void box_blur_h(png_bytep* scl, png_bytep* tcl,int r);
void box_blur_t(png_bytep* scl, png_bytep* tcl,int r);
