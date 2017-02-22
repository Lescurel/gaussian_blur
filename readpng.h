#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <png.h>

int i;

int width, height;
int num_interlaced;
png_bytep row;
png_structp img_struct;
png_infop img_info;
png_byte color, depth;
