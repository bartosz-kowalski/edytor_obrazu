#include "OutputBLock.hpp"

void OutputBlock::saveImage() {

	if (in != nullptr && !filePath.empty()) {
		try {
			fail = 1;
			ExportImage(*in, filePath.c_str());
		}
		catch (...) {
			fail = 2;
			printf("%s", "giga L \n");
		}
	}
	else {
		fail = 2;
		printf("%s", "Brak pod³¹czonego wejœcia");
	}
}

OutputBlock::OutputBlock(std::shared_ptr<Image> input, const std::string fileName) {
	out = nullptr;
	filePath = fileName;
	in = input;
}
OutputBlock::~OutputBlock() = default;

void OutputBlock::setInput(std::shared_ptr<Image> input) {
	in = input;
	if (in) {
		//image = ImageCopy(*in);
		out = nullptr;
	}
}
void OutputBlock::process() {
	wasActive = true;
	saveImage();
	printf("%s", "processed");
}

void OutputBlock::setFilePath(const std::string& path) {
	filePath = path;
}

std::shared_ptr<Image> OutputBlock::getOutput() {
	saveImage();
	return nullptr;
}

const char* OutputBlock::getName() {
	return filePath.c_str();
}

BlockType OutputBlock::getType() const {
	return BlockType::Output;
}

void OutputBlock::Draw() {

	static Texture2D tekstura = LoadTexture("tekstury/Output.png");
	if (wasActive) {
		switch (fail) {
		case 0:
			tekstura = LoadTexture("tekstury/Output.png");
			break;
		case 1:
			tekstura = LoadTexture("tekstury/OutputGit.png");
			break;
		case 2:
			tekstura = LoadTexture("tekstury/OutputSlabo.png");
			break;
		default:
			break;
		}
		wasActive = false;
	}

	DrawCircleV(GetInputPos(), 5, BLUE);

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
