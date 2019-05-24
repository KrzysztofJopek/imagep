#ifndef _MORPH_H_
#define _MORPH_H_

struct image* dilation(int length, double angle, struct image* image_in);
struct image* erosion(int length, double angle, struct image* image_in);
struct image* imclose(int length, double angle, struct image* image_in);

#endif
