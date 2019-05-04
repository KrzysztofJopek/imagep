#include "image.h"
#include <limits.h>

struct size{
	int min_x;
	int max_x;
	int min_y;
	int max_y;
};

struct size size_after_affine(float a1, float a2, float a3, float a4,
	       	int width, int height);

struct image* affine_transform(float a1, float a2, float a3, float a4,
	       	struct image* image_in, int resize)
{
	int bx = 0;
	int by = 0;
	int w = image_in->width;
	int h = image_in->height;
	if(resize){
		struct size size = size_after_affine(a1, a2, a3, a4,
			       	image_in->width, image_in->height);
		
		w = size.max_x - size.min_x;
		h = size.max_y - size.min_y;
		bx = -(size.min_x);
		by = -(size.min_y);
	}

	struct image* image_out = empty_image(w, h, image_in->channels);

	for(int x=0; x<image_in->width; x++){
		for(int y=0; y<image_in->height; y++){
			int pix_x = x*a1 + y*a2 + bx;
			int pix_y = x*a3 + y*a4 + by;
			copy_pixel(image_in, image_out, x, y, pix_x, pix_y);
		}
	}
	return image_out;
}

struct size size_after_affine(float a1, float a2, float a3, float a4,
	       	int width, int height)
{
	int pix_x = 0;
	int pix_y = 0;
	struct size size = {INT_MAX, INT_MIN, INT_MAX, INT_MIN};

	pix_x = 0;
	pix_y = 0;
	if(pix_x > size.max_x)
		size.max_x = pix_x;
	if(pix_x < size.min_x)
		size.min_x = pix_x;
	if(pix_y > size.max_y)
		size.max_y = pix_y;
	if(pix_y < size.min_y)
		size.min_y = pix_y;

	pix_x = width*a1;
	pix_y = width*a3; 
	if(pix_x > size.max_x)
		size.max_x = pix_x;
	if(pix_x < size.min_x)
		size.min_x = pix_x;
	if(pix_y > size.max_y)
		size.max_y = pix_y;
	if(pix_y < size.min_y)
		size.min_y = pix_y;

	pix_x = height*a2;
	pix_y = height*a4;
	if(pix_x > size.max_x)
		size.max_x = pix_x;
	if(pix_x < size.min_x)
		size.min_x = pix_x;
	if(pix_y > size.max_y)
		size.max_y = pix_y;
	if(pix_y < size.min_y)
		size.min_y = pix_y;

	pix_x = width*a1 + height*a2;
	pix_y = width*a3 + height*a4;
	if(pix_x > size.max_x)
		size.max_x = pix_x;
	if(pix_x < size.min_x)
		size.min_x = pix_x;
	if(pix_y > size.max_y)
		size.max_y = pix_y;
	if(pix_y < size.min_y)
		size.min_y = pix_y;

	return size;
}
