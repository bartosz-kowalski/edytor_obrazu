#pragma once
#include "BasicBlock.hpp"

class OutputBlock : public BasicBlock {
private:
	std::string filePath;

	void saveImage();
public:
	OutputBlock(std::shared_ptr<Image> input, const std::string fileName);

	~OutputBlock() override;

	void setInput(std::shared_ptr<Image> input) override;

	void process() override;

	void setFilePath(const std::string& path);

	std::shared_ptr<Image> getOutput() override;
};
