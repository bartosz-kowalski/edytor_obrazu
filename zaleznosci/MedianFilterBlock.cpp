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
	wasActive = true;
	if (in != nullptr) {
		image = ImageCopy(*in);
		//median();
		auto j = std::jthread(&MedianFilterBlock::median, this);
		out = std::make_shared<Image>(image);
		printf("%s", "processed");
		fail = 1;
	}
	else {
		fail = 2;
		printf("%s", "Brak pod³¹czonego wejœcia");
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

BlockType MedianFilterBlock::getType() const {
	return BlockType::Median;
}

void MedianFilterBlock::Draw() {

	static Texture2D tekstura = LoadTexture("tekstury/Median.png");
	if (wasActive) {
		switch (fail) {
		case 0:
			tekstura = LoadTexture("tekstury/Median.png");
			break;
		case 1:
			tekstura = LoadTexture("tekstury/MedianGit.png");
			break;
		case 2:
			tekstura = LoadTexture("tekstury/MedianSlabo.png");
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
