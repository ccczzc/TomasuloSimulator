#pragma once

#include "util.hh"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class TomasuloSimulator {
public:
    size_t clocks_{0};
    size_t raw_stalls_{0};
    size_t war_stalls_{0};
    size_t num_left_instructions_;
    const int loadstore_latency_ = 2;
    const int adddsubd_latency_ = 2;
    const int multd_latency_ = 10;
    const int divd_latency_ = 40;
    std::vector<std::shared_ptr<Instruction>> instructions_;
    std::vector<std::shared_ptr<Station>> loadstore_stations_;
    std::vector<std::shared_ptr<Station>> reservation_stations_;
    std::unordered_map<std::string, std::string> registers_;
    std::unordered_map<std::string, std::string> memory_;
    std::unordered_map<std::string, std::shared_ptr<Station>> register_status_;
    std::vector<SimulatorState> states_;

    TomasuloSimulator() = delete;
    TomasuloSimulator(std::vector<std::shared_ptr<Instruction>>&& instructions, const int num_load_stations = 3, const int num_add_rsstation = 3, const int num_mul_rsstation = 2);
    bool IsFinish() const;
    void PrintInstructions() const;
    void PrintLoadAndReservStations() const;
    void PrintRegisterStatus() const;
    void PrintStatistic() const;
    void PrintAllInfo() const;

    void StoreState();
    void Run();
    void SingleStep();
    void Step(size_t cycles = 1);
    void RunToEnd();
    void Backtrace(size_t cycles = 1);
    
    static void HelpPrintUsage();
    static void HelpPrintInstructions(const std::vector<std::shared_ptr<Instruction>>& instructions, const size_t clocks);
    static void HelpPrintLoadAndReservStations(const std::vector<std::shared_ptr<Station>>& loadstore_stations, const std::vector<std::shared_ptr<Station>>& reservation_stations);
    static void HelpPrintRegisterStatus(const std::unordered_map<std::string, std::shared_ptr<Station>>& register_status);
    static void HelpPrintStatistic(const size_t clocks, const int raw_stalls, const int war_stalls);
    static void HelpPrintReservStation(const std::shared_ptr<Station>& reserv_station);
};