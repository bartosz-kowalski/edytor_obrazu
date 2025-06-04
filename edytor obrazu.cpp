#include "zaleznosci/BasicBlock.hpp"
#include "zaleznosci/InputBlock.hpp"
#include "zaleznosci/OutputBlock.hpp"
#include "zaleznosci/MedianFilterBlock.hpp"
#include "zaleznosci/GaussianFilterBlock.hpp"
#include "zaleznosci/SobelFilterBlock.hpp"
#include "zaleznosci/LaplacianFilterBlock.hpp"

int main() {

	std::vector<OutputBlock> outputs;

	InputBlock in("zdjecie.jpg");
	in.process();

	MedianFilterBlock median(in.getOutput(), 9);
	GaussianFilterBlock gauss(in.getOutput(), 9, 3);
	SobelFilterBlock sobel(in.getOutput(), 100, false);
	LaplacianFilterBlock laplace(in.getOutput(), false);
	median.process();
	gauss.process();
	sobel.process();
	laplace.process();

	OutputBlock o1(median.getOutput(), "median_wyj.jpg");
	OutputBlock o2(gauss.getOutput(), "gauss_wyj.jpg");
	OutputBlock o3(sobel.getOutput(), "sobel_wyj.jpg");
	OutputBlock o4(laplace.getOutput(), "laplace_wyj.jpg");

	o1.process();
	o2.process();
	o3.process();
	o4.process();

	return 0;
}
