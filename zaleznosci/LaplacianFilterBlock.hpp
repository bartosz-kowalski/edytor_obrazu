#pragma once
#include "BasicBlock.hpp"

class LaplacianFilterBlock : public BasicBlock {
private:
	bool negative;
public:
	const BlockType type = BlockType::Laplacian;

	LaplacianFilterBlock(std::shared_ptr<Image>, bool);

	~LaplacianFilterBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	void laplacian();

	void setNeg(bool);

	bool getNeg();

	BlockType getType() const override;
};
