#pragma once
#include "BasicBlock.hpp"

class LaplacianFilterBlock : public BasicBlock {
private:
	bool negative;
public:
	LaplacianFilterBlock(std::shared_ptr<Image> input, bool negative);

	~LaplacianFilterBlock() override;

	void setInput(std::shared_ptr<Image> input) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	void laplacian();

};