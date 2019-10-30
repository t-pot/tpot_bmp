#include <time.h>// clock
#include "../tpot_bmp.h"

int main(int args, int argc, char** argv)
{
	// initialize
	int width = 1920;
	int height = 1080;
	tpot::bmp* bmp = new  tpot::bmp(width, height);

	// write image
	unsigned char* p = bmp->get_image_addr();
	for (int y = 0; y < height; y++) 
	{
		int idx = 3 * width * y;

		for (int x = 0; x < width; x++) 
		{
			p[idx + 0] = 128;             // Blue
			p[idx + 1] = 256 * y / width; // Green
			p[idx + 2] = 256 * x / width; // Red
			idx += 3;
		}
	}

	// save image
	clock_t start = clock(); // start save time measurement

	bmp->save("result.bmp");

	clock_t end = clock();   // finish save time measurement
	printf("save timeF%.2fms\n", (double)(end - start) * 1000.0 / (double)CLOCKS_PER_SEC);

	// finalize
	delete bmp;

	return 0;
}
