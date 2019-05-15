#ifndef _FILTRATION_H_
#define _FILTRATION_H_

#include "image.h"

struct mask{
        double* values;
        int size;
};

struct image* apply_mask(struct image* image_in, struct mask mask,
		double (*filt_func)(struct mask));

struct image* stdfilt(struct image* image);

#endif
