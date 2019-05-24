#include "morph.h"
#include "image.h"
#include <stdio.h>
#include "filtration.h"
#include "math.h"

struct line{
    double* values;
    int x;
    int y;
};

void draw_line(int x1, int y1, int x2, int y2, struct line* line)
{
    double dx,dy,step;
    double x ,y;
    int i;

    dx=abs(x2-x1);
    dy=abs(y2-y1);

    if(dx>=dy)
        step=x2-x1;
    else
        step=y2-y1;

    dx=dx/step;
    dy=dy/step;

    x=x1;
    y=y1;

    i=1;
    while(i<=abs(step)+1)
    {
        *(line->values + (line->x*(int)(abs(y))+ (int)(abs(x)))) = 1;
        x=x+dx;
        y=y+dy;
        i=i+1;
    }

#ifdef _DEBUG_
    for(int x=0; x<line->x; x++){
        for(int y=0; y<line->y; y++){
            printf("%d ", (int)*(line->values + (line->x*(int)(abs(y))+ (int)(abs(x)))));
        }
        printf("\n");
    }
#endif

}

struct line* create_line(int length, int angle)
{
    struct line* line = malloc(sizeof(struct line));
    if(!line)
        return NULL;

    //normalize angle to <0, 180)
    angle %= 180;
    if(angle<0)
        angle += 180;

    int x1;
    int y1;
    int x2;
    int y2;
    if(angle >= 0 && angle < 90){
        x1 = 0;
        y1 = (length-1)*sin(angle*M_PI/180);
        x2 = round(-cos(angle*M_PI/180)*(length-1) + x1);
        y2 = round(y1 - sin(angle*M_PI/180)*(length-1));
    }
    else{
        x1 = 0;
        y1 = 0;
        x2 = round(-cos(angle*M_PI/180)*(length-1) + x1);
        y2 = round(y1 - sin(angle*M_PI/180)*(length-1));
    }
    line->x = abs(x2-x1)+1;
    line->y = abs(y2-y1)+1;
    line->values = calloc(line->x*line->y, sizeof(double));
    if(!line->values)
        return NULL;

    draw_line(x1,y1,x2,y2, line);

    return line;
}

void free_line(struct line* line)
{
    free(line->values);
    free(line);
}

struct image* dilation(int length, double rad, struct image* image_in)
{
    if(image_in->channels != 1){
        fprintf(stderr, "Can't use dilation on multi channel image\n");
        return NULL;
    }
    struct line* line = create_line(length, rad);
    if(!line)
        ERROR();




    free_line(line);
    return NULL;
}



