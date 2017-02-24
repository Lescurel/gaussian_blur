#include <math.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
 ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
   _a < _b ? _a : _b; })

#define CHANNEL 4

int * sizes;
int * pixel_val;

void blur_file();
void box_gauss(double sigman, int n);
void box_blur(png_bytep* scl, png_bytep* tcl, int r);
void box_blur_h(png_bytep* scl, png_bytep* tcl,int r);
void box_blur_t(png_bytep* scl, png_bytep* tcl,int r);
