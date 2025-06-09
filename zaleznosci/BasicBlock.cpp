#include "BasicBlock.hpp"

Vector2 BasicBlock::GetInputPos() const { return { position.x, position.y + 40 }; }

Vector2 BasicBlock::GetOutputPos() const { return { position.x + 150, position.y + 40 }; }

void BasicBlock::Draw() {
    // Rysowanie bloku
    Rectangle rect = { position.x, position.y, 150, 80 };
    DrawRectangleRec(rect, LIGHTGRAY);
    DrawRectangleLinesEx(rect, 2, DARKGRAY);
    DrawText(name.c_str(), position.x + 10, position.y + 10, 10, BLACK);

    // Gniazda
    DrawCircleV(GetInputPos(), 5, BLUE);
    DrawCircleV(GetOutputPos(), 5, RED);

    // Drag & drop
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, rect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dragging = true;
            dragOffset = Vector2Subtract(mouse, position);
        }
    }
    if (dragging) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            position = Vector2Subtract(mouse, dragOffset);
        }
        else {
            dragging = false;
        }
    }
}

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
