#include "simulator.hh"
#include <utility>

TomasuloSimulator::TomasuloSimulator(
    std::vector<std::shared_ptr<Instruction>> &&instructions)
    : instructions_(std::move(instructions)) {}