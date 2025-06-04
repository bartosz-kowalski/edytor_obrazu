#pragma once
#include "BasicBlock.hpp"

class LaplacianFilterBlock : public BasicBlock {
private:
	bool negative;
public:
	LaplacianFilterBlock(std::shared_ptr<Image>, bool);

	~LaplacianFilterBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	void laplacian();

};