/*
 * Thanks to this helpful site :
 * http://blog.ivank.net/fastest-gaussian-blur.html
 *
 */

#include "readpng.h"
#include "blur.h"

void gauss_blur(int radius)
{
  box_gauss(radius, 3);
  box_blur(rows, rows_target,(sizes[0]-1)/2);
  box_blur(rows_target, rows, (sizes[1]-1)/2);
  box_blur(rows, rows_target,(sizes[2]-1)/2);
  //box_blur(rows_target,rows, (sizes[3]-1)/2);
}

void box_gauss(double sigma, int n){
  /* width of the smaller averaging filter */
  int wl = (int)(sqrt((12*sigma*sigma/n)+1));
  /* wl must be odd */
  if(wl%2==0) wl--;
  /* width of the larger averager filter */
  int wu = wl+2;
  /* number of filtering to be done with the smaller filter */
  int m = round((12.0*sigma*sigma -n*wl*wl - 4.0*n*wl -3.0*n)/(-4.0*wl -4.0));
  /* the number of filtering with the larger filter is m-n */
  if(m>n || m<0){
    fprintf(stderr, "%i must be > 0 and < %i \n",m,n);
    exit(EXIT_FAILURE);
  }
  sizes = malloc(sizeof(int)*n);
  for (int i = 0; i < n; i++) {
    if(i<m)
      sizes[i] = wl;
    else
      sizes[i] = wu;
  }
}

void box_blur(png_bytep* scl, png_bytep* tcl,int r){
  box_blur_h(scl,tcl,r);
  box_blur_t(tcl,scl,r);
}

void box_blur_h(png_bytep* scl, png_bytep* tcl,int r){
  double iarr = 1.0/(r+r+1);
  pixel_val = malloc(sizeof(int)*CHANNEL);
  int i=0;
  for (int y = 0; y < height; y++) {
    png_bytep row = scl[y];
    png_bytep row_target = tcl[y];
    png_bytep fv = &(row[0]);
    png_bytep lv = &(row[(width-1)*CHANNEL]);
    int ri=r;
    int ti = 0;
    int li = 0;
    for(i=0;i<CHANNEL;i++)
      pixel_val[i] = (r+1)*fv[i];

    for (int x = 0;x < r;x++) {
      for(i=0;i<CHANNEL;i++){
        png_bytep ptr = &(row[x*4]);
        pixel_val[i] += ptr[i];
      }
    }

    for(x=0;x<=r;x++){
      png_bytep ptr = &(row[(ri++)*4]);
      png_bytep ptr_target = &(row_target[(ti++)*4]);
      for(i=0;i<CHANNEL;i++){
        pixel_val[i] += ptr[i]-fv[i];
        ptr_target[i] = round(pixel_val[i]*iarr);
      }
    }

    for(x=r+1;x<=width-r;x++){
      png_bytep ptr = &(row[(ri++)*4]);
      png_bytep ptr2 = &(row[(li++)*4]);
      png_bytep ptr_target = &(row_target[(ti++)*4]);
      for(i = 0;i<CHANNEL;i++){
        pixel_val[i] += ptr[i]-ptr2[i];
        ptr_target[i] = round(pixel_val[i]*iarr);
      }
    }

    for(x=width-r;x<width;x++){
      png_bytep ptr = &(row[(li++)*4]);
      png_bytep ptr_target = &(row_target[(ti++)*4]);
      for(i=0;i<CHANNEL;i++){
        pixel_val[i] += lv[i] - ptr[i];
        ptr_target[i] = round(pixel_val[i]*iarr);
      }
    }
  }
  free(pixel_val);
}

void box_blur_t(png_bytep* scl, png_bytep* tcl,int r){
  double iarr = 1.0 / (r+r+1);
  pixel_val = malloc(sizeof(int)*CHANNEL);
  int i=0;
  png_bytep first_row = scl[0];
  png_bytep last_row = scl[height-1];
  for(x=0;x<width;x++){
    png_bytep fv = &(first_row[x*4]);
    png_bytep lv = &(last_row[x*4]);

    for(i=0;i<CHANNEL;i++)
      pixel_val[i] = (r+1)*fv[i];

    int ri = r;
    int ti = 0;
    int li = ti;

    for(y=0;y<r;y++){
      png_bytep row = scl[y];
      png_bytep ptr = &(row[x*4]);
      for(i=0;i<CHANNEL;i++){
        pixel_val[i] += ptr[i];
      }
    }

    for(y=0;y<=r;y++){
      png_bytep row_scl = scl[ri];
      png_bytep ptr_scl = &(row_scl[x*4]);
      png_bytep row_tcl = tcl[ti];
      png_bytep ptr_tcl = &(row_tcl[x*4]);
      for(i=0;i<CHANNEL;i++){
        pixel_val[i] += ptr_scl[i] - fv[i];
        ptr_tcl[i] = round(pixel_val[i]*iarr);
      }
      ri++;
      ti++;
    }

    for(y=r+1;y<height-r;y++){
      png_bytep row_scl = scl[ri];
      png_bytep ptr_scl = &(row_scl[x*4]);
      png_bytep row_tcl = tcl[ti];
      png_bytep ptr_tcl = &(row_tcl[x*4]);
      png_bytep row_scl2 = scl[li];
      png_bytep ptr_scl2 = &(row_scl2[x*4]);
      for(i=0;i<CHANNEL;i++){
        pixel_val[i] += ptr_scl[i] - ptr_scl2[i];
        ptr_tcl[i] = round(pixel_val[i]*iarr);
      }
      ri++;
      ti++;
      li++;
    }

    for(y=height-r;y<height;y++){
      png_bytep row_scl = scl[li];
      png_bytep ptr_scl = &(row_scl[x*4]);
      png_bytep row_tcl = tcl[ti];
      png_bytep ptr_tcl = &(row_tcl[x*4]);
      for(i=0;i<CHANNEL;i++){
        pixel_val[i] += lv[i] - ptr_scl[i];
        ptr_tcl[i] = round(pixel_val[i]*iarr);
      }
      li++;
      ti++;
    }
    }
    free(pixel_val);

}

int main(int argc, char* argv[]){
  if(argc == 2){
    read_file(argv[1]);
    gauss_blur(BLUR);
    write_file(argv[1]);
    return 0;
  }
  else if(argc == 3){
    read_file(argv[1]);
    gauss_blur(BLUR);
    write_file(argv[2]);
    return 0;
  }else {
    fprintf(stderr, "You must pass two arguments, the file to be read and the file to be wrote\n");
    return -1;
  }
}
