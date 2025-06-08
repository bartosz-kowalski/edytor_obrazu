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
#include <stack>

enum class BlockType {
	None,
	Input,
	Median,
	Gaussian,
	Laplacian,
	Sobel,
	Output
};

class BasicBlock {
protected:
	Vector2 position;

	std::string name;

	bool dragging = false;

	Vector2 dragOffset;

	Image image;

	std::shared_ptr<Image> in;

	std::shared_ptr<Image> out;

	int idx_prev, idx_next, idx;
public:
	BasicBlock() {};

	virtual ~BasicBlock() = 0;

	virtual void process() = 0;

	virtual void setInput(std::shared_ptr<Image>) = 0;

	virtual std::shared_ptr<Image> getOutput() = 0;

	virtual void Draw();

	virtual void setPosition(Vector2);

    virtual Vector2 GetInputPos() const;

    virtual Vector2 GetOutputPos() const; 

	virtual void setPrev(int);

	virtual void setNext(int);

	virtual int getPrev();

	virtual int getNext();

	virtual int getIdx();

	virtual void setIdx(int);

	virtual BlockType getType() const = 0;
};

inline BasicBlock::~BasicBlock() {
	//UnloadImage(image);
}
