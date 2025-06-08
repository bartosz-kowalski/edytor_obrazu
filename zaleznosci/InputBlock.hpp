#pragma once
#include "BasicBlock.hpp"

class InputBlock : public BasicBlock {
private:
	std::string fileName;

	bool isFileLoaded;

	void loadImage();
public:
	const BlockType type = BlockType::Input;

	InputBlock(const std::string&);

	~InputBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void setFileName(const std::string&);

	void process() override;

	const char* getName();

	std::shared_ptr<Image> getOutput() override;

	BlockType getType() const override;
};
