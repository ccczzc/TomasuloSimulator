#pragma once

#include "util.hh"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class TomasuloSimulator {
    std::vector<std::shared_ptr<Instruction>> instructions_;
    std::vector<std::shared_ptr<LoadStoreStation>> load_stations_;
    std::vector<std::shared_ptr<ReservationStation>> reservation_stations_;
    std::unordered_map<std::string, RegisterStatus> register_status_;

    TomasuloSimulator() = delete;
    TomasuloSimulator(std::vector<std::shared_ptr<Instruction>>&& instructions); 
};