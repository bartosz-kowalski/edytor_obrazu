#pragma once

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <string>
#include <filesystem>
#include <vector>
#include <cmath>
#include <memory>
#include <thread>

class BasicBlock {
protected:
	Image image;
	std::shared_ptr<Image> in;
	std::shared_ptr<Image> out;
public:
	BasicBlock() {};
	virtual ~BasicBlock() = 0;
	virtual void process() = 0;
	virtual void setInput(std::shared_ptr<Image>) = 0;
	virtual std::shared_ptr<Image> getOutput() = 0;
};

inline BasicBlock::~BasicBlock() {
	//UnloadImage(image);
}
