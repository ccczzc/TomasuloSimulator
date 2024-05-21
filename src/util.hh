#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Instruction Type
enum class InstOp {
  LOAD,
  STORE,
  ADDD,
  SUBD,
  MULD,
  DIVD,
  NONE,
};

// Station Type, Add/Mult(ReservationStation), Load(LoadStoreStation)
enum class StationType {
  LOAD,
  ADD,
  MULT,
};

// Station, include
// ReservationStation, for add, sub, mult, div
// LoadStation, for load, store
class Station {
public:
  std::string name_;
  StationType station_type_;
  InstOp instop_;
  bool busy_;
  int time_;
  std::string Vj_;
  std::string Vk_;
  std::shared_ptr<Station> Qj_{nullptr};
  std::shared_ptr<Station> Qk_{nullptr};
  std::string Address_;
  Station() = delete;
  Station(std::string name, StationType station_type);
  ~Station() = default;
  void ResetEmpty();

  std::shared_ptr<Station> Clone() const {
    return std::make_shared<Station>(*this);
  }
};

// Instruction class
class Instruction {
public:
  InstOp instop_;
  std::string text_;
  std::string rd_or_imm_;
  std::string rs_;
  std::string rt_;
  std::string result_;
  int issue_time_{-1};
  int exec_begin_time_{-1};
  int exec_end_time_{-1};
  int write_time_{-1};
  std::shared_ptr<Station> station_{nullptr};
  Instruction() = delete;
  Instruction(InstOp instop, std::string text, std::string rd_or_imm,
              std::string rs, std::string rt);
  ~Instruction() = default;
  std::shared_ptr<Instruction> Clone() const {
    return std::make_shared<Instruction>(*this);
  }
};

class SimulatorState {
public:
  size_t clocks_;
  size_t raw_stalls_;
  size_t war_stalls_;
  size_t num_left_instructions_;
  std::vector<std::shared_ptr<Instruction>> instructions_;
  std::vector<std::shared_ptr<Station>> loadstore_stations_;
  std::vector<std::shared_ptr<Station>> reservation_stations_;
  std::unordered_map<std::string, std::string> registers_;
  std::unordered_map<std::string, std::string> memory_;
  std::unordered_map<std::string, std::shared_ptr<Station>> register_status_;

  SimulatorState() = delete;
  SimulatorState(size_t clocks, size_t raw_stalls, size_t war_stalls,
                 size_t num_left_instructions,
                 std::vector<std::shared_ptr<Instruction>> instructions,
                 std::vector<std::shared_ptr<Station>> loadstore_stations,
                 std::vector<std::shared_ptr<Station>> reservation_stations,
                 std::unordered_map<std::string, std::string> registers,
                 std::unordered_map<std::string, std::string> memory,
                 std::unordered_map<std::string, std::shared_ptr<Station>>
                     register_status);
  ~SimulatorState() = default;
  void PrintAllInfo() const;
};
