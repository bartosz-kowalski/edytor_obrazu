#pragma once
#include "BasicBlock.hpp"

class InputBlock : public BasicBlock {
private:
	std::string fileName;

	bool isFileLoaded;

	void loadImage();
public:
	InputBlock(const std::string&);

	~InputBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void setFileName(const std::string&);

	void process() override;

	std::shared_ptr<Image> getOutput() override;
};
