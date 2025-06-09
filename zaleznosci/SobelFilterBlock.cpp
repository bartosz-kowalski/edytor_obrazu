#include "SobelFilterBlock.hpp"

SobelFilterBlock::SobelFilterBlock(std::shared_ptr<Image> input, int thresh, bool white) : edgeThresh(thresh), bw(white) {
	in = input;
	//image = ImageCopy(*in);
	out = nullptr;
}
SobelFilterBlock::~SobelFilterBlock() = default;
void SobelFilterBlock::setInput(std::shared_ptr<Image> input) {
	in = input;
	if (in) {
		//image = ImageCopy(*in);
		out = nullptr;
	}
}
void SobelFilterBlock::process() {
	if (in != nullptr) {
		image = ImageCopy(*in);
		//sobel();
		std::jthread(&SobelFilterBlock::sobel, this);
		out = std::make_shared<Image>(image);
		printf("%s", "processed");
	}
	else {
		printf("%s", "Brak pod³¹czonego wejœcia");
	}
}
std::shared_ptr<Image> SobelFilterBlock::getOutput() {
	return out;
}
void SobelFilterBlock::sobel() {
	int width = image.width;
	int height = image.height;

	std::vector<std::vector<int>> Gx, Gy;

	Gx = {
		{ -1,  0,  1 },
		{ -2,  0,  2 },
		{ -1,  0,  1 }
	};

	Gy = {
		{ -1, -2, -1 },
		{  0,  0,  0 },
		{  1,  2,  1 }
	};

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
	std::vector<std::vector<float>> grayscale(height, std::vector<float>(width, 0.0f));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Color color = pixels[i * width + j];
			grayscale[i][j] = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
		}
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float gx = 0, gy = 0;
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (x + j >= 0 && j + x < width && y + i >= 0 && i + y < height) {
						float val = grayscale[y + i][x + j];
						gx += val * Gx[i + 1][j + 1];
						gy += val * Gy[i + 1][j + 1];
					}
				}
			}
			float phi = atan2(gy, gx);
			float g = sqrt(pow(gx, 2) + pow(gy, 2));
			unsigned char g_val;
			if (bw)
				g_val = (unsigned char)std::clamp(255.0f - g, 0.0f, 255.0f);
			else
				g_val = (unsigned char)std::clamp(g, 0.0f, 255.0f);
			output[y * width + x] = { g_val,g_val,g_val,255 };
		}
	}

	UnloadImageColors(pixels);
	if (image.data) MemFree(image.data);
	image.data = output;
	image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	image.mipmaps = 1;
}

bool SobelFilterBlock::getBW() {
	return bw;
}

void SobelFilterBlock::setBW(bool BW) {
	bw = BW;
}

int SobelFilterBlock::getThresh() {
	return edgeThresh;
}

void SobelFilterBlock::setThresh(int th) {
	edgeThresh = th;
}

BlockType SobelFilterBlock::getType() const{
	return BlockType::Sobel;
}

void SobelFilterBlock::Draw() {

	static Texture2D tekstura = LoadTexture("tekstury/Sobel.png");

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
