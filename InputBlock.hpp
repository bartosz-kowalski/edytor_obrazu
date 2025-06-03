#pragma once
#include "BasicBlock.hpp"

class InputBlock : public BasicBlock {
private:
	std::string fileName;

	bool isFileLoaded;

	void loadImage();
public:
	InputBlock(const std::string& name);

	~InputBlock() override;

	void setInput(std::shared_ptr<Image> input) override;

	void setFileName(const std::string& name);

	void process() override;

	std::shared_ptr<Image> getOutput() override;
};
