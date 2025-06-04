#include "MedianFilterBlock.hpp"

MedianFilterBlock::MedianFilterBlock(std::shared_ptr<Image> input, int f_size) : filterSize(f_size) {
	in = input;
	//image = ImageCopy(*in);
	out = nullptr;
}
MedianFilterBlock::~MedianFilterBlock() = default;
void MedianFilterBlock::setInput(std::shared_ptr<Image> input) {
	in = input;
	if (in) {
		//image = ImageCopy(*in);
		out = nullptr;
	}
}
void MedianFilterBlock::process() {
	if (in) {
		image = ImageCopy(*in);
		median();
		out = std::make_shared<Image>(image);
	}
}
std::shared_ptr<Image> MedianFilterBlock::getOutput() {
	return out;
}
void MedianFilterBlock::median() {
	int width = image.width;
	int height = image.height;
	int k = filterSize / 2;

	if (image.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) {

		try {
			ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		}
		catch (int errorCode) {
			image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
		}
	}

	Color* pixels = LoadImageColors(image);
	Color* output = (Color*)MemAlloc(width * height * sizeof(Color));

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::vector<unsigned char> r, g, b, a;
			for (int ky = -k; ky <= k; ky++) {
				for (int kx = -k; kx <= k; kx++) {
					int nx = x + kx;
					int ny = y + ky;
					if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
						Color c = pixels[ny * width + nx];
						r.push_back(c.r);
						g.push_back(c.g);
						b.push_back(c.b);
						a.push_back(c.a);
					}
				}
			}
			std::sort(r.begin(), r.end());
			std::sort(g.begin(), g.end());
			std::sort(b.begin(), b.end());
			std::sort(a.begin(), a.end());
			int mid = r.size() / 2;
			output[y * width + x] = { r[mid], g[mid], b[mid], a[mid] };
		}
	}

	UnloadImageColors(pixels);
	if (image.data) MemFree(image.data);
	image.data = output;
	image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	image.mipmaps = 1;
}

void MedianFilterBlock::SetSize(int size) {
	if (size % 2 != 0)
		filterSize = size;
	else
		filterSize = 5;
}

int MedianFilterBlock::getSize() {
	return filterSize;
}
