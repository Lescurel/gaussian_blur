/*
 * This part of the program is heavily based on the work seen here :
 * http://zarb.org/~gc/html/libpng.html
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 */

#include "readpng.h"

void read_file(char* file_name){

        /* To check if its a png file */
        char header[8];

        FILE *fp = fopen(file_name, "rb");
        fread(header,1,8,fp);

        if(png_sig_cmp(header,0,8)) {
                fprintf(stderr, "%s is not a PNG file\n", file_name);
                exit(EXIT_FAILURE);
        }

        img_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

        if(!img_struct) {
                fprintf(stderr, "Cannot create a png structure\n");
                exit(EXIT_FAILURE);
        }

        img_info = png_create_info_struct(img_struct);
        if(!img_info) {
                fprintf(stderr, "Cannot create a png info \n");
                exit(EXIT_FAILURE);
        }

        png_init_io(img_struct,fp);
        png_set_sig_bytes(img_struct,8);
        png_read_info(img_struct,img_info);

        /* retrieving essentials informations */
        width = png_get_image_width(img_struct, img_info);
        height = png_get_image_height(img_struct, img_info);
        color = png_get_color_type(img_struct,img_info);
        depth = png_get_bit_depth(img_struct,img_info);

        if(depth == 16)
                png_set_strip_16(img_struct);

        if(color == PNG_COLOR_TYPE_PALETTE)
                png_set_palette_to_rgb(img_struct);

        /* PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth. */
        if(color == PNG_COLOR_TYPE_GRAY && depth < 8)
                png_set_expand_gray_1_2_4_to_8(img_struct);

        if(png_get_valid(img_struct, img_info, PNG_INFO_tRNS))
                png_set_tRNS_to_alpha(img_struct);

        /* These color type don't have an alpha channel then fill it with 0xff. */
        if(color == PNG_COLOR_TYPE_RGB ||
           color == PNG_COLOR_TYPE_GRAY ||
           color == PNG_COLOR_TYPE_PALETTE)
                png_set_filler(img_struct, 0xFF, PNG_FILLER_AFTER);

        if(color == PNG_COLOR_TYPE_GRAY ||
           color == PNG_COLOR_TYPE_GRAY_ALPHA)
                png_set_gray_to_rgb(img_struct);

        png_read_update_info(img_struct, img_info);

        rows = (png_bytep*) malloc(sizeof(png_bytep) * height);
        rows_target = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (i = 0; i < height; i++){
                rows[i] = (png_byte*) malloc(png_get_rowbytes(img_struct,img_info));
                rows_target[i] = (png_byte*) malloc(png_get_rowbytes(img_struct,img_info));
              }

        png_read_image(img_struct,rows);

        fclose(fp);
}

void write_file(char* file_name){
        FILE* fp = fopen(file_name,"wb");
        if(!fp) {
                fprintf(stderr, "Couldn't write %s file\n",file_name);
                exit(EXIT_FAILURE);
        }

        img_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
        if(!img_struct) {
                fprintf(stderr, "Couldn't create a png structure\n");
                exit(EXIT_FAILURE);
        }

        img_info = png_create_info_struct(img_struct);
        if(!img_info) {
                fprintf(stderr, "Couldn't create a png info \n");
                exit(EXIT_FAILURE);
        }

        png_init_io(img_struct,fp);

        png_set_IHDR(img_struct, img_info, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(img_struct, img_info);

        png_write_image(img_struct,rows);

        png_write_end(img_struct,NULL);

        /* For some reason, I got a double free or corruption error with some images.
        Since it's the end of the program, it doesn't matter very much if I remove those ligns,
        but it's strange */
        // for (i = 0; i < height; i++){
        //         free(rows[i]);
        //         free(rows_target[i]);
        // }
        // free(rows);
        // free(rows_target);

        fclose(fp);
}
