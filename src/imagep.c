#include "image.h"
#include "filtration.h"
#include "morph.h"

int main(int argc, char* argv[])
{
    struct image* img = load_image(argv[1], 3);
    dilation(10, 170, img);
    
	return 0;
}
