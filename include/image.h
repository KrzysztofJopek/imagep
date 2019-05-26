#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>

#define ERROR() do{fprintf(stderr, "ERROR OCCURED\n"); exit(1);}while(0)

struct image{
	unsigned char* data;
	int width;
	int height;
	int channels;
};

struct image* load_image(const char* path, int channels);
int save_image(struct image* image);
void free_image(struct image* image);
struct image* empty_image(int x, int y, int channels);
unsigned char* get_pixel(const struct image* image, int x, int y);
int copy_pixel(struct image* image_in, struct image* image_out,
	       	int src_x, int src_y, int dst_x, int dst_y);
void copy_image(struct image* image_in, struct image* image_out);


#endif
