#include "zaleznosci/BasicBlock.hpp"

struct Connection {
    std::shared_ptr<BasicBlock> from;
    std::shared_ptr<BasicBlock> to;
    int idx_form;
    int idx_to;
};
