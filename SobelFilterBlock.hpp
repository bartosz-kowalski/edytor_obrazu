#pragma once
#include "BasicBlock.hpp"

class SobelFilterBlock : public BasicBlock {
private:
	int edgeThresh = 100;
public:
	SobelFilterBlock(std::shared_ptr<Image> input, int thresh);

	~SobelFilterBlock() override;

	void setInput(std::shared_ptr<Image> input) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	void sobel();
};
