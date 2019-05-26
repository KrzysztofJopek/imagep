#include "image.h"
#include "filtration.h"
#include "morph.h"

int main(int argc, char* argv[])
{
    struct image* img = load_image(argv[1], 1);
    save_image(convex_hull(img));
    
	return 0;
}
