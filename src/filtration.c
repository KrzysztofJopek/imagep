#include "filtration.h"
#include <math.h>
#include <stdio.h>

void get_mult_mask(struct mask mask, struct mask* values, struct image* image,
        int x, int y, int channel);

/*
 * Apply mask to the image and return output image,
 * You have to specify filtration function as defined filt_func
 * if extend flag is true, then this function copy image_in
 * and inserts only white pixels
 */ 
struct image* apply_mask(struct image* image_in, struct mask mask,
        double (*filt_func)(struct mask), int extend)
{
    struct image* image_out = empty_image(image_in->width, image_in->height,
            image_in->channels);
    if(extend)
        copy_image(image_in, image_out);
    
    int center = mask.size/2;
    double data[mask.size*mask.size];
    struct mask temp = {data, mask.size};

    for(int x=0; x<image_in->width - mask.size; x++){
        for(int y=0; y<image_in->height - mask.size; y++){
            for(int ch=0; ch<image_in->channels; ch++){
                get_mult_mask(mask, &temp, image_in, x, y, ch);
                if(!extend){
                    *(get_pixel(image_out, x+center, y+center)+ch) = (*filt_func)(temp)*255;
                }
                else{
                    if((*filt_func)(temp) >= 0.9){
                        printf("XD\n");
                        *(get_pixel(image_out, x+center, y+center)+ch) = 0xff;
                    }
                }
            }
        }
    }
    return image_out;
}

/*
 * Multiply masked part of image by mask(for one channel),
 * and return output in values struct
 */
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

/*
 * One of possible filter functions passed to apply_mask
 * Calculates value of new pixel from given "masked" region
 */
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

/*
 * public function
 * Creates mask and runs std filtration
 */
struct image* stdfilt(struct image* image)
{
    double data[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    struct mask mask = {data, 5};
    return apply_mask(image, mask, std_filt, 0);
}

/*
 * convex hull filatration
 */
static int hull_changed = 1;
double hull_filt(struct mask values)
{
    int plus = 0;
    for(int i=0; i<9; i++){
        if(values.values[i] > 0.9)
            plus++;
        if(values.values[i] < -0.9)
            return 0;
    }
    if(plus == 4){
        hull_changed = 1;
        return 1;
    }
    return 0;
}

/*
 * convex hull effect
 */
struct image* convex_hull(struct image* image)
{
    double data[8][9] = {
        {1,1,0,1,-1,0,1,0,-1},
        {1,1,1,0,-1,1,-1,0,0},
        {-1,0,1,0,-1,1,0,1,1},
        {0,0,-1,1,-1,0,1,1,1},
        {0,1,1,0,-1,1,-1,0,1},
        {-1,0,0,0,-1,1,1,1,1},
        {1,0,-1,1,-1,0,1,1,0},
        {1,1,1,1,-1,0,0,0,-1}
    };
    struct image* image_prev = NULL;
    struct image* image_out = image;
    struct mask mask = {NULL, 3};
    hull_changed = 1;
    while(hull_changed == 1){
        hull_changed = 0;
        for(int i=0; i<8; i++){
            image_prev = image_out;
            mask.values = data[i];
            image_out = apply_mask(image_prev, mask, hull_filt, 1);
            if(image_prev != image)
                free_image(image_prev);
        }
    }
    return image_out;
}

