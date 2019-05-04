#ifndef _IMAGE_H_
#define _IMAGE_H_

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

#endif
