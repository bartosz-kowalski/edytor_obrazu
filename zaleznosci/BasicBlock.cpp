#include "BasicBlock.hpp"

Vector2 BasicBlock::GetInputPos() const { return { position.x, position.y + 40 }; }

Vector2 BasicBlock::GetOutputPos() const { return { position.x + 150, position.y + 40 }; }

void BasicBlock::setPosition(Vector2 pos) {
    position = pos;
}

void BasicBlock::setPrev(int p) {
    idx_prev = p;
}

void BasicBlock::setNext(int n) {
    idx_next = n;
}

int BasicBlock::getPrev() const{
    return idx_prev;
}

int BasicBlock::getNext() const{
    return idx_next;
}

int BasicBlock::getIdx() const{
    return idx;
}

void BasicBlock::setIdx(int i) {
    idx = i;
}

void BasicBlock::wait() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    fail = 0;
    wasActive = true;
}
