#pragma once

#include "util.hh"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class TomasuloSimulator {
public:
    int clocks_{0};
    bool is_finished_{false};
    const int loadstore_latency = 2;
    const int adddsubd_latency = 2;
    const int multd_latency = 10;
    const int divd_latency = 40;
    std::vector<std::shared_ptr<Instruction>> instructions_;
    std::vector<std::shared_ptr<Station>> loadstore_stations_;
    std::vector<std::shared_ptr<Station>> reservation_stations_;
    std::unordered_map<std::string, std::string> registers_;
    std::unordered_map<std::string, std::string> memory_;
    std::unordered_map<std::string, std::shared_ptr<Station>> register_status_;


    TomasuloSimulator() = delete;
    TomasuloSimulator(std::vector<std::shared_ptr<Instruction>>&& instructions, const int num_load_stations = 3, const int num_add_rsstation = 3, const int num_mul_rsstation = 2);
    void PrintInstructions() const;
    void PrintLoadAndReservStations() const;
    void PrintRegisterStatus() const;
    void PrintStatistic() const;
    void PrintAllInfo() const;

    void Run();
    void SingleStep();
    void Step(size_t cycles = 1);
    void RunToEnd();
    void Backtrace(size_t cycles = 1);
    static void PrintUsage();
    static void PrintReservStation(const std::shared_ptr<Station>& reserv_station);
};