#include "image.h"
//stb_image to load image from file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//stb_image_write to write image to file
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
	if(image->data){
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

