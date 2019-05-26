#include "image.h"
//stb_image to load image from file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//stb_image_write to write image to file
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string.h>

/*
 * Gets pointer to pixel from image.
 * Return NULL in case of error
 */
unsigned char* get_pixel(const struct image* image, int x, int y)
{
	if(x < 0 || y < 0 || x >= image->width || y >= image->height)
		return NULL;

	return image->data + (image->width*y + x)*image->channels;
}

/*
 * loads image from path.
 * Return pointer to new image structure,
 * NULL in case of earror
 */
struct image* load_image(const char* path, int channels)
{
	struct image* image = (struct image*)malloc(sizeof(struct image));
	if(!image)
		return NULL;

	image->data = stbi_load(path, &image->width, &image->height,
			&image->channels, channels);
	if(!image->data){
		free(image);
		return NULL;
	}
	return image;
}


/*
 * Saves image to output.bmp
 *TODO redirect image to STDOUT
 */
int save_image(struct image* image)
{
	return stbi_write_bmp("output.bmp", image->width, image->height,
			image->channels, image->data);
}

/*
 * Create empty image struct
 */
struct image* empty_image(int x, int y, int channels)
{
	struct image* image = (struct image*)malloc(sizeof(struct image));
	if(!image)
		return NULL;

	image->data = (unsigned char*)malloc(
			sizeof(unsigned char)*x*y*channels);
	if(!image->data){
		free(image);
		return NULL;
	}
	image->width = x;
	image->height = y;
	image->channels = channels;
	return image;
}

void free_image(struct image* image)
{
	free(image->data);
	free(image);
}

/*
 * Copy pixel from image_in to image_out
 * if src_pixel or dst_pixel is out of range return value will be 0.
 * Both images must have the same number of channels, otherwise result
 * is undefined and possibly buffer overflow.
 */
int copy_pixel(struct image* image_in, struct image* image_out,
	       	int src_x, int src_y, int dst_x, int dst_y)
{
	unsigned char* src_pixel = get_pixel(image_in, src_x, src_y);
	unsigned char* dst_pixel = get_pixel(image_out, dst_x, dst_y);
	if(!dst_pixel || !src_pixel)
		return 0;

	memcpy(dst_pixel, src_pixel, sizeof(unsigned char)*image_in->channels);
	return 1;
}

void copy_image(struct image* image_in, struct image* image_out)
{
    memcpy(image_out->data, image_in->data, sizeof(unsigned char)*
            image_out->width * image_out->height * image_out->channels);
}
