#include "GaussianFilterBlock.hpp"

GaussianFilterBlock::GaussianFilterBlock(std::shared_ptr<Image> input, int f_size, float sigma) : filterSize(f_size), sigma(sigma) {
	in = input;
	//image = ImageCopy(*in);
	out = nullptr;
}
GaussianFilterBlock::~GaussianFilterBlock() = default;
void GaussianFilterBlock::setInput(std::shared_ptr<Image> input) {
	in = input;
	if (in) {
		//image = ImageCopy(*in);
		out = nullptr;
	}
}
void GaussianFilterBlock::process() {
	wasActive = true;
	if (in != nullptr) {
		image = ImageCopy(*in);
		//gaussian();
		auto j = std::jthread(&GaussianFilterBlock::gaussian, this);
		out = std::make_shared<Image>(image);
		printf("%s", "processed");
		fail = 1;
	}
	else {
		fail = 2;
		printf("%s", "Brak pod³¹czonego wejœcia");
	}
}
std::shared_ptr<Image> GaussianFilterBlock::getOutput() {
	return out;
}

std::vector<std::vector<float>> GaussianFilterBlock::generateKernel() {
	std::vector<std::vector<float>> kernel(filterSize, std::vector<float>(filterSize, 0.0f));
	float sum = 0.0f;

	int k = filterSize / 2;
	float sigma_sqr = sigma * sigma;
	float coeff = 1.0f / (2.0f * PI * sigma_sqr);

	for (int i = -k; i <= k; i++) {
		for (int j = -k; j <= k; j++) {
			float value = coeff * exp(-(i * i + j * j) / (2 * sigma_sqr));
			kernel[i + k][j + k] = value;
			sum += value;
		}
	}
	for (int i = 0; i < filterSize; i++) {
		for (int j = 0; j < filterSize; j++) {
			kernel[i][j] /= sum;
		}
	}
	return kernel;
}

void GaussianFilterBlock::gaussian() {

	int width = image.width;
	int height = image.height;
	int k = filterSize / 2;

	std::vector<std::vector<float>> kernel = generateKernel();

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

float GaussianFilterBlock::getSigma() {
	return sigma;
}

void GaussianFilterBlock::setSigma(float s) {
	sigma = s;
}

int GaussianFilterBlock::getSize() {
	return filterSize;
}

void GaussianFilterBlock::setSize(int size) {
	if (size % 2 != 0)
		filterSize = size;
	else
		size = 5;
}

BlockType GaussianFilterBlock::getType() const {
	return BlockType::Gaussian;
}

void GaussianFilterBlock::Draw() {

	static Texture2D tekstura = LoadTexture("tekstury/Gaussian.png");
	if (wasActive) {
		switch (fail) {
		case 0:
			tekstura = LoadTexture("tekstury/Gaussian.png");
			break;
		case 1:
			tekstura = LoadTexture("tekstury/GaussianGit.png");
			break;
		case 2:
			tekstura = LoadTexture("tekstury/GaussianSlabo.png");
			break;
		default:
			break;
		}
		wasActive = false;
	}

	DrawCircleV(GetInputPos(), 5, BLUE);
	DrawCircleV(GetOutputPos(), 5, RED);

	DrawTexture(tekstura, position.x, position.y, WHITE);

	Vector2 mouse = GetMousePosition();

	static Rectangle textureRect;
	textureRect.x = position.x;
	textureRect.y = position.y;
	textureRect.width = (float)tekstura.width;
	textureRect.height = (float)tekstura.height;


	if (CheckCollisionPointRec(mouse, textureRect)) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			dragging = true;
			dragOffset = Vector2Subtract(mouse, position);
		}
	}
	if (dragging) {
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			position = Vector2Subtract(mouse, dragOffset);
		}
		else {
			dragging = false;
		}
	}
}
