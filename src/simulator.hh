#pragma once

#include "util.hh"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class TomasuloSimulator {
public:
    size_t clocks_;

    std::vector<std::shared_ptr<Instruction>> instructions_;
    std::vector<std::shared_ptr<LoadStoreStation>> load_stations_;
    std::vector<std::shared_ptr<ReservationStation>> reservation_stations_;
    std::unordered_map<std::string, RegisterStatus> register_status_;


    TomasuloSimulator() = delete;
    TomasuloSimulator(std::vector<std::shared_ptr<Instruction>>&& instructions, const int num_load_stations = 3, const int num_add_rsstation = 3, const int num_mul_rsstation = 2);
    void PrintInstructions() const;
    void PrintLoadAndReservStations() const;
    void PrintRegisterStatus() const;
    void PrintStatistic() const;
    void PrintAllInfo() const;

    void Run();
    void Step(size_t cycles = 1);
    void RunToEnd();
    void Backtrace(size_t cycles = 1);
    static void PrintUsage();
};