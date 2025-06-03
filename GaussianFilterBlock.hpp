#pragma once
#include "BasicBlock.hpp"

class GaussianFilterBlock : public BasicBlock {
private:
	int filterSize = 3;

	float sigma = 3;
public:
	GaussianFilterBlock(std::shared_ptr<Image> input, int f_size, float sigma);

	~GaussianFilterBlock() override;

	void setInput(std::shared_ptr<Image> input) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	std::vector<std::vector<float>> generateKernel();

	void gaussian();
};
