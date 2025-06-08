#pragma once
#include "BasicBlock.hpp"

class OutputBlock : public BasicBlock {
private:
	std::string filePath;

	void saveImage();
public:
	const BlockType type = BlockType::Output;

	OutputBlock(std::shared_ptr<Image>, const std::string);

	~OutputBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void process() override;

	void setFilePath(const std::string&);

	const char* getName();

	std::shared_ptr<Image> getOutput() override;

	BlockType getType() const override;
};
