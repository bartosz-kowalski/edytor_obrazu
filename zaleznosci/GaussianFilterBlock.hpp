#pragma once
#include "BasicBlock.hpp"

class GaussianFilterBlock : public BasicBlock {
private:
	int filterSize = 3;

	float sigma = 3;
public:
	const BlockType type = BlockType::Gaussian;

	GaussianFilterBlock(std::shared_ptr<Image>, int, float);

	~GaussianFilterBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	std::vector<std::vector<float>> generateKernel();

	void gaussian();

	float getSigma();

	void setSigma(float);

	int getSize();

	void setSize(int);

	BlockType getType() const override;

	void Draw() override;
};
