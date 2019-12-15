#include "bmp.h"
#include "movie.h"

#include <cstdio>
#include <sstream>
#include <vector>

#define FPS 25

using namespace std;

static void saveBmp(int width, int height, uint8_t* pixels,
	const string& filename)
{
	vector<char> vbmp(BMP_SIZE(width, height));
	char* bmp = reinterpret_cast<char*>(&vbmp[0]);
	bmp_init(bmp, width, height);
	
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			// The pixels array is in RGBA format, that is x4.
			const unsigned long color =
				*(unsigned long*)&pixels[(j * width + i) * 4];
			bmp_set(bmp, i, j, color);
		}
	
	FILE* fp = fopen(filename.c_str(), "wb");
    fwrite(bmp, vbmp.size(), 1, fp);
    fclose(fp);
}

int main(int argc, char* argv[])
{
	const unsigned int width = 1280;
	const unsigned int height = 720;

	const char* filename = "stouza";
	MovieWriter movie(filename, width, height);

	{
		MovieReader prototype("../media/part1", width, height);
		vector<uint8_t> pixels;
		for (int iframe = 0; ; iframe++)
		{
			if (!prototype.getFrame(pixels))
				break;

			movie.addFrame(&pixels[0]);
#if 0			
			stringstream ss;
			ss << "frame_";
			ss << iframe;
			ss << ".bmp";
			saveBmp(width, height, &pixels[0], ss.str());
#endif
		}
	}

	{
		const unsigned int nframes = FPS * 10;
		for (unsigned int iframe = 0; iframe < nframes; iframe++)
			movie.addFrame("../media/part2.png");
	}

	return 0;
}

