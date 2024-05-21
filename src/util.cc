#include "util.hh"
#include "simulator.hh"
#include <algorithm>
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
    : instop_(instop), text_(std::move(text)), rd_or_imm_(rd_or_imm), rs_(rs),
      rt_(rt), issue_time_(-1), exec_begin_time_(-1), exec_end_time_(-1),
      write_time_(-1), station_(nullptr) {}

SimulatorState::SimulatorState(
    size_t clocks, size_t raw_stalls, size_t war_stalls,
    size_t num_left_instructions,
    std::vector<std::shared_ptr<Instruction>> instructions,
    std::vector<std::shared_ptr<Station>> loadstore_stations,
    std::vector<std::shared_ptr<Station>> reservation_stations,
    std::unordered_map<std::string, std::string> registers,
    std::unordered_map<std::string, std::string> memory,
    std::unordered_map<std::string, std::shared_ptr<Station>> register_status)
    : clocks_(clocks), raw_stalls_(raw_stalls), war_stalls_(war_stalls), num_left_instructions_(num_left_instructions),
      instructions_(instructions), loadstore_stations_(loadstore_stations),
      reservation_stations_(reservation_stations), registers_(registers),
      memory_(memory), register_status_(register_status) {}

void SimulatorState::PrintAllInfo() const {
TomasuloSimulator::HelpPrintInstructions(instructions_, clocks_);
TomasuloSimulator::HelpPrintLoadAndReservStations(loadstore_stations_, reservation_stations_);
TomasuloSimulator::HelpPrintRegisterStatus(register_status_);
TomasuloSimulator::HelpPrintStatistic(clocks_, raw_stalls_, war_stalls_);
}