#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

struct image* affine_transform(float a1, float a2, float a3, float a4, 
	       	struct image* image_in, int resize);

#endif
