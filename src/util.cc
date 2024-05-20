#include "util.hh"
#include <cstddef>
#include <utility>

Station::Station(std::string name, StationType station_type)
    : name_(name), station_type_(station_type), instop_(InstOp::NONE),
      busy_(false) {}

ReservationStation::ReservationStation(std::string name,
                                       StationType station_type)
    : Station(name, station_type), time_(-1),
    Qj_(nullptr), Qk_(nullptr) {}

LoadStoreStation::LoadStoreStation(std::string name, StationType station_type)
    : Station(name, station_type) {}

RegisterStatus::RegisterStatus() : busy_(false), FU_(nullptr) {}

Instruction::Instruction(InstOp instop, std::string text)
    : instop_(instop), text_(std::move(text)), issue_time_(-1),
      exec_begin_time_(-1), exec_end_time_(-1), write_time_(-1) {}

RegisterInst::RegisterInst(InstOp instop, std::string text, std::string rd,
                           std::string rs, std::string rt)
    : Instruction(instop, text), rd_(std::move(rd)), rs_(std::move(rs)),
      rt_(std::move(rt)) {}

LoadStoreInst::LoadStoreInst(InstOp instop, std::string text, std::string rt,
                             std::string imm, std::string rs)
    : Instruction(instop, text), rt_(std::move(rt)), imm_(std::move(imm)),
      rs_(std::move(rs)) {}