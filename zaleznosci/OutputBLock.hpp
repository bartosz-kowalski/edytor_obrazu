#pragma once
#include "BasicBlock.hpp"

class OutputBlock : public BasicBlock {
private:
	std::string filePath;

	void saveImage();
public:
	OutputBlock(std::shared_ptr<Image>, const std::string);

	~OutputBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void process() override;

	void setFilePath(const std::string&);

	std::shared_ptr<Image> getOutput() override;
};
