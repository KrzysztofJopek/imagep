#include "filtration.h"
#include <math.h>
#include <stdio.h>

void get_mult_mask(struct mask mask, struct mask* values, struct image* image,
		int x, int y, int channel);

struct image* apply_mask(struct image* image_in, struct mask mask,
		double (*filt_func)(struct mask))
{
	struct image* image_out = empty_image(image_in->width, image_in->height,
            image_in->channels);
	int center = mask.size/2;
    double data[mask.size*mask.size];
	struct mask temp = {data, mask.size};
    
	for(int x=0; x<image_in->width - mask.size; x++){
		for(int y=0; y<image_in->height - mask.size; y++){
			for(int ch=0; ch<image_in->channels; ch++){
				get_mult_mask(mask, &temp, image_in, x, y, ch);
				*(get_pixel(image_out, x+center, y+center)+ch) = (*filt_func)(temp)*256;
                //printf("%d\n", *(get_pixel(image_out, x+center, y+center)+ch));
			}
		}
	}
    return image_out;
}

void get_mult_mask(struct mask mask, struct mask* values, struct image* image,
		int x, int y, int channel)
{
	double* p = values->values;
	for(int i=x; i<x+mask.size; i++){
		for(int j=y; j<y+mask.size; j++){
			unsigned char* pix = get_pixel(image, i, j);
			*p++ = (double)(*(pix+channel))/256 * *mask.values++;
		}
	}
}

double std_filt(struct mask values)
{
    //calculate mean
    double mean = 0;
    int size = values.size*values.size;
    double* p = values.values;
    for(int i=0; i<size; i++){
        mean += *p++;
    }
    mean /= size;

    //calculate std
    p = values.values;
    double res = 0;
    for(int i=0; i<size; i++){
        res += pow(*p - mean, 2);
        p++;
    }
    return sqrt(res/(size-1));
}

double new_filt(struct mask values)
{
    return values.values[1];
}

struct image* stdfilt(struct image* image)
{
    double data[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    struct mask mask = {data, 5};
    return apply_mask(image, mask, std_filt);
}
