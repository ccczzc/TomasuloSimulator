#include "simulator.hh"
#include "util.hh"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "You did not provide MIPS file.\n"
              << "Usage: ./Simulator [your  MIPS assembly code file]\n";
    exit(-1);
  }
  std::vector<std::shared_ptr<Instruction>> instructions;
  std::ifstream fin(argv[1]);
  std::string curr_codeline;
  while (std::getline(fin, curr_codeline)) {
    std::istringstream iss(curr_codeline);
    std::string opcode;
    iss >> opcode;
    InstOp instop;
    if (opcode == "L.D")
      instop = InstOp::LOAD;
    else if (opcode == "S.D")
      instop = InstOp::STORE;
    else if (opcode == "ADD.D")
      instop = InstOp::ADDD;
    else if (opcode == "SUB.D")
      instop = InstOp::SUBD;
    else if (opcode == "MUL.D")
      instop = InstOp::MULD;
    else if (opcode == "DIV.D")
      instop = InstOp::DIVD;
    else {
      std::cerr << "Operation " << opcode << " is not support!\n";
      exit(-1);
    }
    std::string op1, op2, op3;
    iss >> op1 >> op2 >> op3;
    switch (instop) {
    case InstOp::LOAD:
    case InstOp::STORE:
      instructions.push_back(
          std::make_shared<Instruction>(instop, curr_codeline, op2, op3, op1));
      break;
    case InstOp::ADDD:
    case InstOp::SUBD:
    case InstOp::MULD:
    case InstOp::DIVD:
      instructions.push_back(
          std::make_shared<Instruction>(instop, curr_codeline, op1, op2, op3));
      break;
    default:
      break;
    }
  }
  TomasuloSimulator mysim(std::move(instructions));
  mysim.Run();
  return 0;
}