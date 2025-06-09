#include "InputBlock.hpp"

void InputBlock::loadImage() {
	if (!fileName.empty()) {
		try {
			image = LoadImage(fileName.c_str());
			out = std::make_shared<Image>(image);
			isFileLoaded = true;
		}
		catch (...) {
			out = nullptr;
			isFileLoaded = false;
		}
	}
	else {
		out = nullptr;
		isFileLoaded = false;
	}
}

InputBlock::InputBlock(const std::string& name) {
	in = nullptr;
	isFileLoaded = false;
	fileName = name;
}

InputBlock::~InputBlock() = default;

void InputBlock::setInput(std::shared_ptr<Image> input) {
	in = nullptr;
}

void InputBlock::setFileName(const std::string& name) {
	fileName = name;
	loadImage();
}

void InputBlock::process() {
	loadImage();
	printf("%s", "processed");
}

std::shared_ptr<Image> InputBlock::getOutput() {
	return out;
}

const char* InputBlock::getName() {
	return fileName.c_str();
}

BlockType InputBlock::getType() const {
	return BlockType::Input;
}

int InputBlock::getPrev() const {
	return -1;
}

void InputBlock::Draw() {

	static Texture2D tekstura = LoadTexture("tekstury/Input.png");

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
