#include "image.h"
//stb_image to load image from file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	if(!image->data)
		return NULL;
	return image;
}

void free_image(struct image* image)
{
	stbi_image_free(image->data);
	free(image);
}
