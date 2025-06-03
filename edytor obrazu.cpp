#include "InputBlock.hpp"
#include "OutputBlock.hpp"
#include "MedianFilterBlock.hpp"
#include "GaussianFilterBlock.hpp"
#include "SobelFilterBlock.hpp"
#include "LaplacianFilterBlock.hpp"

int main() {

	std::vector<InputBlock> inputs;
	std::vector<BasicBlock> filters;
	std::vector<std::thread> threads;
	std::vector<OutputBlock> outputs;

	inputs.push_back(InputBlock("dziekan.jpg"));
	inputs[0].process();

	filters.push_back(MedianFilterBlock(inputs[0].getOutput(), 9));
	filters.push_back(GaussianFilterBlock(inputs[0].getOutput(), 9, 3));
	filters.push_back(SobelFilterBlock(inputs[0].getOutput(), 100));
	filters.push_back(LaplacianFilterBlock(inputs[0].getOutput(), true));


	threads.push_back(std::thread(&MedianFilterBlock::process, &filters[0]));
	threads.push_back(std::thread(&GaussianFilterBlock::process, &filters[1]));
	threads.push_back(std::thread(&SobelFilterBlock::process, &filters[2]));
	threads.push_back(std::thread(&LaplacianFilterBlock::process, &filters[3]));

	for (std::thread& thread : threads) {
		thread.join();
	}


	outputs.push_back(OutputBlock(filters[0].getOutput(), "median_wyj.jpg"));
	outputs.push_back(OutputBlock(filters[1].getOutput(), "gauss_wyj.jpg"));
	outputs.push_back(OutputBlock(filters[2].getOutput(), "sobel_wyj.jpg"));
	outputs.push_back(OutputBlock(filters[3].getOutput(), "laplace_wyj.jpg"));

	for (OutputBlock& out : outputs) {
		out.process();
	}

	return 0;
}
