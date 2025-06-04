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
	InitWindow(1280, 720, "Bloczkowy Edytor z raygui");
	SetTargetFPS(60);

	std::vector<BlockWrapper> blocks;
	BlockWrapper* selectedBlock = nullptr;
	std::vector<Connection> connections;
	std::shared_ptr<BasicBlock> connectionStart = nullptr;

	BlockType draggingType = BlockType::None;
	bool dragging = false;

	while (!WindowShouldClose()) {
		bool isCheckedS = true;
		bool isCheckedL = false;
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Pasek narzędzi
		DrawRectangle(0, 0, 140, GetScreenHeight(), LIGHTGRAY);
		DrawText("BLOKI", 10, 10, 20, DARKGRAY);

		if (GuiButton({ 10, 40, 120, 30 }, "Input")) draggingType = BlockType::Input;
		if (GuiButton({ 10, 80, 120, 30 }, "Output")) draggingType = BlockType::Output;
		if (GuiButton({ 10, 120, 120, 30 }, "Sobel")) draggingType = BlockType::Sobel;
		if (GuiButton({ 10, 160, 120, 30 }, "Laplacian")) draggingType = BlockType::Laplacian;
		if (GuiButton({ 10, 200, 120, 30 }, "Median")) draggingType = BlockType::Median;
		if (GuiButton({ 10, 240, 120, 30 }, "Gaussian")) draggingType = BlockType::Gaussian;

		// Drag & drop nowego bloku
		if (draggingType != BlockType::None && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			Vector2 pos = GetMousePosition();
			pos.x = std::max(pos.x, 160.0f);  // nie upuszczaj na menu

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
				//newBlock->process();
				blocks.push_back({ newBlock, draggingType });
				newBlock->setPosition(pos);
				newBlock->Draw();  // pozycja ustawiana przez użytkownika w drag
			}

			draggingType = BlockType::None;
		}

		// Rysowanie i wybór bloków
		for (auto& bw : blocks) {
			bw.block->Draw();

			// Wybór aktywnego bloku
			if (CheckCollisionPointRec(GetMousePosition(), { bw.block->GetInputPos().x, bw.block->GetInputPos().y - 20, 150, 80 })) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					selectedBlock = &bw;
				}
			}
		}

		for (auto& block : blocks) {
			Vector2 mouse = GetMousePosition();

			// Kliknięcie wyjścia: start połączenia
			if (CheckCollisionPointCircle(mouse, block.block->GetOutputPos(), 6)) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					connectionStart = block.block;
				}
			}

			// Kliknięcie wejścia: zakończenie połączenia
			if (CheckCollisionPointCircle(mouse, block.block->GetInputPos(), 6)) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && connectionStart && connectionStart != block.block) {
					// Twórz połączenie
					connections.push_back({ connectionStart, block.block });
					block.block->setInput(connectionStart->getOutput());
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

		// Panel parametrów
		DrawRectangle(1100, 0, 180, GetScreenHeight(), Fade(LIGHTGRAY, 0.3f));
		if (selectedBlock != nullptr) {
			DrawText("PARAMETRY", 1110, 20, 18, DARKGRAY);
			int y = 60;

			switch (selectedBlock->type) {
			case BlockType::Sobel: {
				auto sobel = std::dynamic_pointer_cast<SobelFilterBlock>(selectedBlock->block);
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
					//DrawText(isCheckedS ? "Zaznaczone" : "Odznaczone", 130, 100, 20, DARKGRAY);
					sobel->setBW(bw);
				}
			} break;

			case BlockType::Laplacian: {
				auto lap = std::dynamic_pointer_cast<LaplacianFilterBlock>(selectedBlock->block);
				if (lap) {
					bool neg = lap->getNeg();
					isCheckedL = neg;
					neg = GuiCheckBox({ 1110, (float)y, 20, 20 }, "Negative", &isCheckedL);
					//DrawText(isCheckedL ? "Zaznaczone" : "Odznaczone", 130, 100, 20, DARKGRAY);
					neg = isCheckedL;
					lap->setNeg(neg);
				}
			} break;

			case BlockType::Input: {
				auto input = std::dynamic_pointer_cast<InputBlock>(selectedBlock->block);
				static char filename[128] = "";
				GuiTextBox({ 1110, (float)y, 150, 25 }, filename, 128, true);
				if (GuiButton({ 1110.0f, y + 40.0f, 150.0f, 30.0f }, "Load Image")) {
					input->setFileName(filename);
					input->process();
				}
			} break;

			case BlockType::Output: {
				auto output = std::dynamic_pointer_cast<OutputBlock>(selectedBlock->block);
				static char filename[128] = "";
				GuiTextBox({ 1110, (float)y, 150, 25 }, filename, 128, true);
				if (GuiButton({ 1110.0f, float(y) + 40, 150, 30 }, "Export Image")) {
					output->setFilePath(filename);
					output->process();
				}
			} break;

			case BlockType::Median: {
				auto median = std::dynamic_pointer_cast<MedianFilterBlock>(selectedBlock->block);
				if (median) {
					int size = median->getSize();
					float sizebuf;
					size = GuiSlider({ 1110, (float)y, 20, 20 }, "Size", " ", &sizebuf, 0, 100);
					median->SetSize(3 + static_cast<int>(sizebuf / 100 * 8));
				}
			} break;

			case BlockType::Gaussian: {
				auto gauss = std::dynamic_pointer_cast<GaussianFilterBlock>(selectedBlock->block);
				if (gauss) {
					int size = gauss->getSize();
					float sizebuf;
					float sigma = gauss->getSigma();
					size = GuiSlider({ 1110, (float)y, 20, 20 }, "Size", " ", &sizebuf, 0, 100);
					sigma = GuiSlider({ 1110, (float)y, 40, 20 }, "Sigma", " ", &sigma, 0, 100);
					gauss->setSize(3 + static_cast<int>(sizebuf / 100 * 8));
					gauss->setSigma(sigma / 20);
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

/*int main() {

	InputBlock in("zdjecie.jpg");
	in.process();

	MedianFilterBlock median(in.getOutput(), 5);
	GaussianFilterBlock gauss(in.getOutput(), 5, 3);
	SobelFilterBlock sobel(in.getOutput(), 100, false);
	LaplacianFilterBlock laplace(in.getOutput(), false);
	std::jthread(&BasicBlock::process, &median);
	std::jthread(&BasicBlock::process, &gauss);
	std::jthread(&BasicBlock::process, &sobel);
	std::jthread(&BasicBlock::process, &laplace);

	OutputBlock o1(median.getOutput(), "median_wyj.jpg");
	OutputBlock o2(gauss.getOutput(), "gauss_wyj.jpg");
	OutputBlock o3(sobel.getOutput(), "sobel_wyj.jpg");
	OutputBlock o4(laplace.getOutput(), "laplace_wyj.jpg");

	o1.process();
	o2.process();
	o3.process();
	o4.process();

	return 0;
}*/
