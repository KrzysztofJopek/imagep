#include "image.h"
#include "filtration.h"

int main(int argc, char* argv[])
{
    struct image* img = load_image(argv[1], 3);
    save_image(stdfilt(img));
	return 0;
}
