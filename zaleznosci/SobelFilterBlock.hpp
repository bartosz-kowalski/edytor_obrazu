#pragma once
#include "BasicBlock.hpp"

class SobelFilterBlock : public BasicBlock {
private:
	int edgeThresh = 100;
	bool bw = true;
public:
	SobelFilterBlock(std::shared_ptr<Image>, int, bool);

	~SobelFilterBlock() override;

	void setInput(std::shared_ptr<Image>) override;

	void process() override;

	std::shared_ptr<Image> getOutput() override;

	void sobel();

	bool getBW();

	void setBW(bool);

	int getThresh();

	void setThresh(int);

};
