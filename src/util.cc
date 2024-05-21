#include "util.hh"
#include <cstddef>
#include <utility>

Station::Station(std::string name, StationType station_type)
    : name_(name), station_type_(station_type), instop_(InstOp::NONE),
      busy_(false), time_(-1), Qj_(nullptr), Qk_(nullptr) {}

void Station::ResetEmpty() {
  instop_ = InstOp::NONE;
  busy_ = false;
  time_ = -1;
  Vj_.clear();
  Vk_.clear();
  Qj_ = nullptr;
  Qk_ = nullptr;
  Address_.clear();
}
Instruction::Instruction(InstOp instop, std::string text, std::string rd_or_imm,
              std::string rs, std::string rt)
    : instop_(instop), text_(std::move(text)), rd_or_imm_(rd_or_imm), rs_(rs), rt_(rt), issue_time_(-1),
      exec_begin_time_(-1), exec_end_time_(-1), write_time_(-1), station_(nullptr) {}
