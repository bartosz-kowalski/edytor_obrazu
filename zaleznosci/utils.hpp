#include "zaleznosci/BasicBlock.hpp"

enum class BlockType {
    None,
    Input,
    Median,
    Gaussian,
    Laplacian,
    Sobel,
    Output
};

struct BlockWrapper {
    std::shared_ptr<BasicBlock> block;
    BlockType type;
};

struct Connection {
    std::shared_ptr<BasicBlock> from;
    std::shared_ptr<BasicBlock> to;
};
