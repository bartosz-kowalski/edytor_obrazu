#define _CRT_SECURE_NO_WARNINGS
#include "zaleznosci/BasicBlock.hpp"
#include "zaleznosci/InputBlock.hpp"
#include "zaleznosci/OutputBlock.hpp"
#include "zaleznosci/MedianFilterBlock.hpp"
#include "zaleznosci/GaussianFilterBlock.hpp"
#include "zaleznosci/SobelFilterBlock.hpp"
#include "zaleznosci/LaplacianFilterBlock.hpp"
#include "zaleznosci/utils.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main() {
	InitWindow(1280, 720, "Bloczkowy Edytor Obrazow");
	SetTargetFPS(60);

	std::vector< std::shared_ptr<BasicBlock>> blocks;
	std::shared_ptr<BasicBlock> selectedBlock = nullptr;
	std::vector<Connection> connections;
	std::shared_ptr<BasicBlock> connectionStart = nullptr;

	BlockType draggingType = BlockType::None;
	bool dragging = false;

	while (!WindowShouldClose()) {
		bool isCheckedS = true;
		bool isCheckedL = false;
		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawRectangle(0, 0, 140, GetScreenHeight(), LIGHTGRAY);
		DrawText("BLOKI", 10, 10, 20, DARKGRAY);

		if (GuiButton({ 10, 40, 120, 30 }, "Input")) draggingType = BlockType::Input;
		if (GuiButton({ 10, 80, 120, 30 }, "Output")) draggingType = BlockType::Output;
		if (GuiButton({ 10, 120, 120, 30 }, "Sobel")) draggingType = BlockType::Sobel;
		if (GuiButton({ 10, 160, 120, 30 }, "Laplacian")) draggingType = BlockType::Laplacian;
		if (GuiButton({ 10, 200, 120, 30 }, "Median")) draggingType = BlockType::Median;
		if (GuiButton({ 10, 240, 120, 30 }, "Gaussian")) draggingType = BlockType::Gaussian;

		if (draggingType != BlockType::None && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			Vector2 pos = GetMousePosition();
			pos.x = std::max(pos.x, 160.0f);

			std::shared_ptr<BasicBlock> newBlock;
			switch (draggingType) {
			case BlockType::Input: newBlock = std::make_shared<InputBlock>(""); break;
			case BlockType::Output: newBlock = std::make_shared<OutputBlock>(nullptr, ""); break;
			case BlockType::Sobel: newBlock = std::make_shared<SobelFilterBlock>(nullptr, 100, true); break;
			case BlockType::Laplacian: newBlock = std::make_shared<LaplacianFilterBlock>(nullptr, false); break;
			case BlockType::Median: newBlock = std::make_shared<MedianFilterBlock>(nullptr, 3); break;
			case BlockType::Gaussian: newBlock = std::make_shared<GaussianFilterBlock>(nullptr, 3, 1); break;
			default: break;
			}

			if (newBlock) {
				newBlock->setInput(nullptr);
				blocks.push_back({ newBlock});
				newBlock->setPosition(pos);
				newBlock->Draw();
				newBlock->setIdx(static_cast<int>(blocks.size()) - 1);
			}

			draggingType = BlockType::None;
		}

		for (auto& bw : blocks) {
			bw->Draw();

			if (CheckCollisionPointRec(GetMousePosition(), { bw->GetInputPos().x, bw->GetInputPos().y - 20, 150, 80 })) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					selectedBlock = bw;
				}
			}
		}

		for (auto& block : blocks) {
			Vector2 mouse = GetMousePosition();

			if (CheckCollisionPointCircle(mouse, block->GetOutputPos(), 6)) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					connectionStart = block;
				}
			}

			if (CheckCollisionPointCircle(mouse, block->GetInputPos(), 6)) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && connectionStart && connectionStart != block) {
					connections.push_back({ connectionStart, block });
					block->setInput(connectionStart->getOutput());
					block->setPrev(connectionStart->getIdx());
					connectionStart = nullptr;
				}
			}

			if (connectionStart) {
				DrawLineBezier(
					connectionStart->GetOutputPos(),
					GetMousePosition(),
					2.0f, MAROON
				);
			}

		}

		for (const auto& conn : connections) {
			DrawLineBezier(
				conn.from->GetOutputPos(),
				conn.to->GetInputPos(),
				2.0f, DARKBLUE
			);
		}

		DrawRectangle(1100, 0, 180, GetScreenHeight(), Fade(LIGHTGRAY, 0.3f));
		if (selectedBlock != nullptr) {
			DrawText("PARAMETRY", 1110, 20, 18, DARKGRAY);
			int y = 60;

			switch (selectedBlock->getType()) {
			case BlockType::Sobel: {
				auto sobel = std::dynamic_pointer_cast<SobelFilterBlock>(selectedBlock);
				if (sobel) {
					static int thresh = 100;
					thresh = sobel->getThresh();
					GuiSlider({ 1110, (float)y, 150, 20 }, "Threshold", NULL, (float*)&thresh, 0, 255);
					sobel->setThresh(thresh);

					y += 40;
					bool bw = sobel->getBW();
					isCheckedS = bw;
					bw = GuiCheckBox({ 1110, (float)y, 20, 20 }, "B/W mode", &isCheckedS);
					bw = isCheckedS;
					sobel->setBW(bw);
					if (GuiButton({ 1110.0f, y + 40.0f, 150.0f, 30.0f }, "Process")) {
						sobel->setInput(blocks[sobel->getPrev()]->getOutput());
						sobel->process();
					}
				}
			} break;

			case BlockType::Laplacian: {
				auto lap = std::dynamic_pointer_cast<LaplacianFilterBlock>(selectedBlock);
				if (lap) {
					bool neg = lap->getNeg();
					isCheckedL = neg;
					neg = GuiCheckBox({ 1110, (float)y, 20, 20 }, "Negative", &isCheckedL);
					neg = isCheckedL;
					lap->setNeg(neg);
					if (GuiButton({ 1110.0f, y + 40.0f, 150.0f, 30.0f }, "Process")) {
						lap->setInput(blocks[lap->getPrev()]->getOutput());
						lap->process();
					}
				}

			} break;

			case BlockType::Input: {
				auto input = std::dynamic_pointer_cast<InputBlock>(selectedBlock);
				static char filename[128] = "";
				//strcpy(filename, input->getName());
				GuiTextBox({ 1110, (float)y, 150, 25 }, filename, 128, true);
				if (GuiButton({ 1110.0f, y + 40.0f, 150.0f, 30.0f }, "Load Image")) {
					input->setFileName(filename);
					input->process();
				}
			} break;

			case BlockType::Output: {
				auto output = std::dynamic_pointer_cast<OutputBlock>(selectedBlock);
				static char filename[128] = "";
				std::stack<int> stos;
				//strcpy(filename, output->getName());
				GuiTextBox({ 1110, (float)y, 150, 25 }, filename, 128, true);
				if (GuiButton({ 1110.0f, float(y) + 40, 150, 30 }, "Export Image")) {
					output->setInput(blocks[output->getPrev()]->getOutput());
					output->setFilePath(filename);

					output->process();
				}
			} break;

			case BlockType::Median: {
				auto median = std::dynamic_pointer_cast<MedianFilterBlock>(selectedBlock);
				if (median) {
					int size = median->getSize();
					float sizebuf;
					size = GuiSlider({ 1110, (float)y, 150, 20 }, "Size", " ", &sizebuf, 0, 100);
					median->SetSize(3 + static_cast<int>(sizebuf / 100 * 8));
					if (GuiButton({ 1110.0f, y + 40.0f, 150.0f, 30.0f }, "Process")) {
						median->setInput(blocks[median->getPrev()]->getOutput());
						median->process();
					}
				}
			} break;

			case BlockType::Gaussian: {
				auto gauss = std::dynamic_pointer_cast<GaussianFilterBlock>(selectedBlock);
				if (gauss) {
					int size = gauss->getSize();
					float sizebuf;
					float sigma = gauss->getSigma();
					size = GuiSlider({ 1110, (float)y, 150, 20 }, "Size", " ", &sizebuf, 0, 100);
					sigma = GuiSlider({ 1110, (float)y + 40.0f, 150, 20 }, "Sigma", " ", &sigma, 0, 100);
					gauss->setSize(3 + static_cast<int>(sizebuf / 100 * 8));
					gauss->setSigma(sigma / 20);
					if (GuiButton({ 1110.0f, y + 80.0f, 150.0f, 30.0f }, "Process")) {
						gauss->setInput(blocks[gauss->getPrev()]->getOutput());
						gauss->process();
					}
				}
			} break;

			default: break;
			}
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
