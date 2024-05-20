#include "simulator.hh"
#include "util.hh"
#include <iostream>
#include <memory>
#include <string>
#include <utility>

std::string InstOpToStr(InstOp instop);

TomasuloSimulator::TomasuloSimulator(
    std::vector<std::shared_ptr<Instruction>> &&instructions, const int num_load_stations, const int num_add_rsstation, const int num_mul_rsstation)
    : clocks_(0), instructions_(std::move(instructions)) {
        for (int i = 1; i <= num_load_stations; ++i) {
            load_stations_.push_back(std::make_shared<LoadStoreStation>("Load"+std::to_string(i), StationType::LOAD));
        }
        for (int i = 1; i <= num_add_rsstation; ++i) {
            reservation_stations_.push_back(std::make_shared<ReservationStation>("Add"+std::to_string(i), StationType::ADD));
        }
        for (int i = 1; i <= num_mul_rsstation; ++i) {
            reservation_stations_.push_back(std::make_shared<ReservationStation>("Mult"+std::to_string(i), StationType::MULT));
        }
        for (int i = 0; i <= 30; i += 2) {
            register_status_["F" + std::to_string(i)] = RegisterStatus();
        }
    }

void TomasuloSimulator::PrintInstructions() const {
    std::cout << "Instruction Status\n";
    std::cout << "Instruction\t\t\tj\tk\tIssue\tExec\tWrite\n";
    for (auto& inst : instructions_) {
        std::cout << inst->text_ << '\t';
        if (inst->issue_time_ != -1) std::cout << inst->issue_time_;
        std::cout << '\t';
        if (inst->exec_begin_time_ != -1) {
            std::cout << inst->exec_begin_time_ << "~";
            if (inst->exec_end_time_ != -1) std::cout << inst->exec_end_time_;
        }
        std::cout << '\t';
        if (inst->write_time_ != -1) std::cout << inst->write_time_;
        std::cout << "\t\n";
    }
}
void TomasuloSimulator::PrintStations() const {
    std::cout << "Load Stations\n";
    std::cout << "Name\t\tBusy\tAddress\t\n";
    for (auto& load_station : load_stations_) {
        std::cout << load_station->name_ << "\t\t" << load_station->busy_ << '\t' << load_station->Address_ << "\t\n";
    }

    std::cout << "Reservation Stations\n";
    std::cout << "Time\tName\t\tBusy\tOp\tVj\tVk\tQj\tQk\t\n";
    for (auto& reserv_station : reservation_stations_) {
        if (reserv_station->time_ != -1) std::cout << reserv_station->time_;
        std::cout << '\t';
        std::cout << reserv_station->name_ << "\t\t";
        std::cout << reserv_station->busy_ << '\t';
        if (reserv_station->busy_ == false) {
            std::cout << "\t\n";
            continue;
        }
        std::cout << InstOpToStr(reserv_station->instop_) << '\t';
        std::cout << reserv_station->Vj_ << '\t' << reserv_station->Vk_ << '\t';
        if (reserv_station->Qj_ != nullptr) {
            std::cout << reserv_station->Qj_->name_;
        }
        std::cout << '\t';
        if (reserv_station->Qk_ != nullptr) {
            std::cout << reserv_station->Qk_->name_;
        }
        std::cout << "\t\n";
    }
}
void TomasuloSimulator::PrintRegisterStatus() const {
    std::cout << "Register Result Status\n";
    std::cout << "Reg\t\t";
    for (int i = 0; i <= 30; i += 2) {
        std::cout << "F" << i << '\t';
    }
    std::cout << '\n';
    std::cout << "FU\t\t";
    for (int i = 0; i <= 30; i += 2) {
        std::string fu = "F" + std::to_string(i);
        auto& reg_status = register_status_.at(fu);
        if (reg_status.busy_) {
            std::cout << reg_status.FU_->name_;
        }
        std::cout << '\t';
    }
    std::cout << "\t\n";
}


std::string InstOpToStr(InstOp instop) {
    std::string ans;
    switch (instop) {
        case InstOp::LOAD:
            ans = "L.D";
            break;
        case InstOp::STORE:
            ans = "S.D";
            break;
        case InstOp::ADDD:
            ans = "ADD.D";
            break;
        case InstOp::SUBD:
            ans = "SUB.D";
            break;
        case InstOp::MULD:
            ans = "MUL.D";
            break;
        case InstOp::DIVD:
            ans = "DIV.D";
            break;
        case InstOp::NONE:
            ans = "NONE";
            break;
    }
    return ans;
}