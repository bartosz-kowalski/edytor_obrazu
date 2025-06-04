#pragma once
#include "BasicBlock.hpp"

class MedianFilterBlock : public BasicBlock {
private:
	int filterSize = 5;
public:
	MedianFilterBlock(std::shared_ptr<Image>, int);

	~MedianFilterBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	void median();

	void SetSize(int);

	int getSize();
};