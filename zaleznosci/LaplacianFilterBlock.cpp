#include "LaplacianFilterBlock.hpp"

LaplacianFilterBlock::LaplacianFilterBlock(std::shared_ptr<Image> input, bool positive) : negative(positive) {
	in = input;
	//image = ImageCopy(*in);
	out = nullptr;
}
LaplacianFilterBlock::~LaplacianFilterBlock() = default;

void LaplacianFilterBlock::setInput(std::shared_ptr<Image> input) {
	in = input;
	if (in) {
		//image = ImageCopy(*in);
		out = nullptr;
	}
}
void LaplacianFilterBlock::process() {
	if (in!=nullptr) {
		image = ImageCopy(*in);
		laplacian();
		out = std::make_shared<Image>(image);
		printf("%s", "processed");
	}
	else {
		printf("%s", "Brak pod³¹czonego wejœcia");
	}
}
std::shared_ptr<Image> LaplacianFilterBlock::getOutput() {
	return out;
}

void LaplacianFilterBlock::laplacian() {

	int width = image.width;
	int height = image.height;
	int k = 3 / 2;
	std::vector<std::vector<int>> kernel;

	if (negative) {
		kernel = {
			{0, 1, 0},
			{1, -4, 1},
			{0, 1, 0}
		};
	}
	else {
		kernel = {
			{0, -1, 0},
			{-1, 4, -1},
			{0, -1, 0}
		};
	}

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
			float r = 0, g = 0, b = 0, a = 0;
			for (int ky = -k; ky <= k; ky++) {
				for (int kx = -k; kx <= k; kx++) {
					int nx = x + kx;
					int ny = y + ky;
					if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
						float waga = kernel[ky + k][kx + k];
						Color c = pixels[ny * width + nx];

						r += c.r * waga;
						g += c.g * waga;
						b += c.b * waga;
						a += c.a * waga;

					}
				}
			}
			output[y * width + x] = {
		(unsigned char)std::clamp(int(r), 0, 255),
		(unsigned char)std::clamp(int(g), 0, 255),
		(unsigned char)std::clamp(int(b), 0, 255),
		(unsigned char)std::clamp(int(a), 0, 255)
			};
		}
	}

	UnloadImageColors(pixels);
	if (image.data) MemFree(image.data);
	image.data = output;
	image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	image.mipmaps = 1;
}

void LaplacianFilterBlock::setNeg(bool neg) {
	negative = neg;
}

bool LaplacianFilterBlock::getNeg() {
	return negative;
}
