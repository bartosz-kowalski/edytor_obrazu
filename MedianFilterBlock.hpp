#pragma once
#include "BasicBlock.hpp"

class MedianFilterBlock : public BasicBlock {
private:
	int filterSize = 5;
public:
	MedianFilterBlock(std::shared_ptr<Image> input, int f_size);

	~MedianFilterBlock() override;

	void setInput(std::shared_ptr<Image> input) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	void median();
};