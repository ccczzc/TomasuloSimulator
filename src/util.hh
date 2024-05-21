#pragma once

#include <cstddef>
#include <memory>
#include <string>

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
};



// Base Instruction class
class Instruction {
public:
  InstOp instop_;
  std::string text_;
  int issue_time_{-1};
  int exec_begin_time_{-1};
  int exec_end_time_{-1};
  int write_time_{-1};
  std::shared_ptr<Station> station_{nullptr};
  Instruction() = delete;
  Instruction(InstOp instop, std::string text);
  virtual ~Instruction() = default;
};

// Register type Instruction class
class RegisterInst : public Instruction {
public:
  std::string rd_;
  std::string rs_;
  std::string rt_;

  RegisterInst() = delete;
  RegisterInst(InstOp instop, std::string text, std::string rd, std::string rs,
               std::string rt);
  virtual ~RegisterInst() = default;
};

// Load/Store type Instruction class
class LoadStoreInst : public Instruction {
public:
  std::string rt_;
  std::string imm_;
  std::string rs_;

  LoadStoreInst() = delete;
  LoadStoreInst(InstOp instop, std::string text, std::string rt,
                std::string imm, std::string rs);
  virtual ~LoadStoreInst() = default;
};