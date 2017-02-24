#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <png.h>

int i,x,y;

int width, height;
png_bytep* rows;
png_bytep* rows_target;
png_structp img_struct;
png_infop img_info;
png_byte color, depth;

/**
 * write a png into a file
 * @file_name : name of the file to be wrote
 */
void write_file(char* file_name);
/**
 * read a png file
 * @file_name : name of the file_name to be read
 */
void read_file(char* file_name);
