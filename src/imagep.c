#include "image.h"
#include "filtration.h"
#include "transform.h"
#include "morph.h"
#include "argp.h"
#include <string.h>

static char doc[] =
    "Simple implementation of few image processing algorithms";

static struct argp_option options[] = {
  {"format", 't', "FORMAT", 0, "Specify format: RGB/GRAY" },
  {"affine", 'a', "MATRIX", 0, "Affine transform"},
  {"std", 's', "SIZE", 0, "STD filtration"},
  {"close", 'c', "LENGTH", 0, "close image"},
  {"angle", 'i', "ANGLE", 0, "line angle"},
  {"convex-hull", 'h', 0, 0, "convex hull"},
  {"normalize", 'n', 0, 0, "Normalize"},
  {"multiple", 'm', 0, 0, "Output image for every channel"},
  { 0 }
};

static char args_doc[] = "INPUT";

struct arguments{
    char* input;
    char* output;
    int channels;
    int affine;
    double affine_matrix[4];
    int std;
    int normalize;
    int multiple;
    int close_length;
    int close_angle;
    int convex;
};

int parse_affine(char* str, struct arguments* args)
{
    char* fEnd = NULL;
    char* ar = strtok(str, ":");
    if(!ar)
        return 0;
    double r = strtod(ar, &fEnd);
    if((*fEnd))
        return 0;
    args->affine_matrix[0] = r;

    for(int i=1; i<4; i++){
        ar = strtok(NULL, ":");
        if(!ar)
            return 0;
        r = strtod(ar, &fEnd);
        if((*fEnd))
            return 0;
        args->affine_matrix[i] = r;
    }
    return 1;
}

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    char* fEnd = NULL;

    switch (key)
    {
        case 'a':
            arguments->affine = 1;
            parse_affine(arg, arguments);
        case 'o':
            arguments->output = arg;
            break;

        case 't':
            if(strcmp(arg, "RGB") == 0)
                arguments->channels = 3;
            else if(strcmp(arg, "GRAY") == 0)
                arguments->channels = 1;
            else if(strcmp(arg, "LOG") == 0)
                arguments->channels = 1;
            else
                argp_usage(state);
            break;

        case 's':
            arguments->std = strtol(arg, &fEnd, 10);
            if((*fEnd))
                return 1;
            if(arguments->std < 0 || arguments->std % 2 == 0){
                fprintf(stderr, "mask has to be odd\n");
                return 1;
            }
            break;

        case 'n':
            arguments->normalize = 1;
            break;

        case 'm':
            arguments->multiple = 1;
            break;

        case 'c':
            arguments->close_length = strtol(arg, &fEnd, 10);
            if((*fEnd))
                return 1;
            if(arguments->close_length <= 0){
                fprintf(stderr, "Wrong length");
                return 1;
            }
            break;

        case 'i':
            arguments->close_angle = strtol(arg, &fEnd, 10);
            if((*fEnd))
                return 1;
            if(arguments->close_angle < 0){
                fprintf(stderr, "Wrong angle");
                return 1;
            }
            break;
        case 'h':
            arguments->convex = 1;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)
                argp_usage (state);

            arguments->input = arg;
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1)
                argp_usage (state);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, NULL  };

int main(int argc, char* argv[])
{
    struct arguments arguments;
    memset(&arguments, 0, sizeof(struct arguments));
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    int howmany = 0;
    if(arguments.convex)
        howmany++;
    if(arguments.std)
        howmany++;
    if(arguments.close_length)
        howmany++;
    if(arguments.affine)
        howmany++;
    if(howmany>1){
        fprintf(stderr, "You can't use these options together\n");
        exit(1);
    }
    if(howmany==0){
        fprintf(stderr, "You didn't specify any option\n");
        exit(1);
    }


    struct image* img = load_image(arguments.input, arguments.channels);
    if(!img){
        fprintf(stderr, "Couldn't load image\n");
        exit(1);
    }
    struct image* img_out;
    if(arguments.affine){
        double* mx = arguments.affine_matrix;
        img_out = affine_transform(
                mx[0], mx[1], mx[2], mx[3], img, 1);
    }
    else if(arguments.std){
        img_out = stdfilt(img, arguments.std);
    }
    else if(arguments.close_length){
        img_out = imclose(arguments.close_length, arguments.close_angle, img);
    }
    else if(arguments.convex){
        img_out = convex_hull(img);
    }

    if(arguments.normalize)
        normalize(img_out);
    if(arguments.multiple){
        for(int i=0; i<img_out->channels; i++){
            struct image* mimg = getChannel(img_out, i);
            if(mimg)
                save_image(mimg, i);
        }
    }
    else{
        save_image(img_out, 0);
    }
    free_image(img);
    free_image(img_out);
	return 0;
}
