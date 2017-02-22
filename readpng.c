#include "readpng.h"

void read_file(char * file_name){

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
        color = png_get_image_color_type(img_struct,img_info);
        depth = png_get_image_bit_depth(img_struct,img_info);

        num_interlaced = png_set_interlace_handling(img_struct);
        png_read_update_info(img_struct, img_info);

        row = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (i = 0; i < height; i++)
          row[i] = (png_bytep*) malloc(png_get_rowbytes(img_struct,img_info));


        png_read_image(img_struct,rows);

        fclose(fp);
}

void write_file(char * file_name){
  FILE *fp = fopen(file_name,"wb");
  if(!fp){
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

  png_set_IHDR(img_struct, img_info, width, height, depth, color, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(img_struct, img_info);

  png_write_image(img_info,rows);

  png_write_end(img_info,NULL);

  for (i = 0; i < height; i++)
    free(rows[i]);
  free(rows);

  fclose(fp);
}

int main(){
        return 0;
}
