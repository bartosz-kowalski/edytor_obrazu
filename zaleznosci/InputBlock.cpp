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

void InputBlock::process(){
	loadImage();
	printf("%s", "processed");
}

std::shared_ptr<Image> InputBlock::getOutput() {
	return out;
}
