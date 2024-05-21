#include "simulator.hh"
#include "util.hh"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

std::string InstOpToStr(InstOp instop);

TomasuloSimulator::TomasuloSimulator(
    std::vector<std::shared_ptr<Instruction>> &&instructions,
    const int num_load_stations, const int num_add_rsstation,
    const int num_mul_rsstation)
    : clocks_(0), is_finished_(false), instructions_(std::move(instructions)) {
  for (int i = 1; i <= num_load_stations; ++i) {
    loadstore_stations_.push_back(std::make_shared<Station>(
        "Load" + std::to_string(i), StationType::LOAD));
  }
  for (int i = 1; i <= num_add_rsstation; ++i) {
    reservation_stations_.push_back(
        std::make_shared<Station>("Add" + std::to_string(i), StationType::ADD));
  }
  for (int i = 1; i <= num_mul_rsstation; ++i) {
    reservation_stations_.push_back(std::make_shared<Station>(
        "Mult" + std::to_string(i), StationType::MULT));
  }
  for (int i = 0; i <= 30; i += 2) {
    std::string curr_reg = "F" + std::to_string(i);
    registers_[curr_reg] = "Regs[" + curr_reg + "]";
    register_status_[curr_reg] = nullptr;
  }
  for (int i = 0; i < 32; ++i) {
    std::string curr_reg = "R" + std::to_string(i);
    registers_[curr_reg] = "Regs[" + curr_reg + "]";
    register_status_[curr_reg] = nullptr;
  }
}

void TomasuloSimulator::PrintInstructions() const {
  std::cout << "Instruction Status\n";
  std::cout << "Instruction\t\tIssue\tExec\tWrite\n";
  for (auto &inst : instructions_) {
    std::cout << inst->text_ << '\t';
    if (inst->issue_time_ != -1)
      std::cout << inst->issue_time_;
    std::cout << '\t';
    if (inst->exec_begin_time_ != -1 && inst->exec_begin_time_ <= clocks_) {
      std::cout << inst->exec_begin_time_ << "~";
      if (inst->exec_end_time_ != -1)
        std::cout << inst->exec_end_time_;
    }
    std::cout << '\t';
    if (inst->write_time_ != -1)
      std::cout << inst->write_time_;
    std::cout << "\t\n";
  }
}
void TomasuloSimulator::PrintLoadAndReservStations() const {
  std::cout << "Load Stations\n";
  std::cout << "Name\t\tBusy\tAddress\t\n";
  for (auto &load_station : loadstore_stations_) {
    std::cout << load_station->name_ << "\t\t" << load_station->busy_ << '\t'
              << load_station->Address_ << "\t\n";
  }

  std::cout << "Reservation Stations\n";
  std::cout << "Time\tName\t\tBusy\tOp\tVj\t\tVk\t\tQj\tQk\t\n";
  for (auto &reserv_station : reservation_stations_) {
    PrintReservStation(reserv_station);
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
    auto &reg_status = register_status_.at(fu);
    if (reg_status != nullptr) {
      std::cout << reg_status->name_;
    }
    std::cout << '\t';
  }
  std::cout << "\t\n";
}

void TomasuloSimulator::PrintStatistic() const {
  std::cout << "Statistic\n";
  std::cout << "Total:\n\t" << clocks_ << " cycle clocks executed\n";
}
void TomasuloSimulator::PrintAllInfo() const {
  PrintInstructions();
  PrintLoadAndReservStations();
  PrintRegisterStatus();
  PrintStatistic();
}
void TomasuloSimulator::Run() {
  PrintInstructions();
  PrintLoadAndReservStations();
  PrintRegisterStatus();
  PrintUsage();
  bool is_terminated = false;
  while (!is_terminated) {
    std::cout << "Please input your command: ";
    std::string cmd;
    getline(std::cin, cmd);
    std::istringstream iss(cmd);
    char cmd_type;
    iss >> cmd_type;
    switch (cmd_type) {
    case 'v':
      char view_type;
      if (!(iss >> view_type)) {
        TomasuloSimulator::PrintUsage();
        break;
      }
      switch (view_type) {
      case 'i':
        PrintInstructions();
        break;
      case 'l':
        PrintLoadAndReservStations();
        break;
      case 'r':
        PrintRegisterStatus();
        break;
      case 's':
        PrintStatistic();
        break;
      case 'a':
        PrintAllInfo();
        break;
      default:
        TomasuloSimulator::PrintUsage();
        break;
      }
      break;
    case 's':
      int steps;
      if (!(iss >> steps)) {
        Step();
      } else {
        Step(steps);
      }
      // PrintAllInfo();
      break;
    case 'r':
      RunToEnd();
      break;
    case 'b':
      int backs;
      if (!(iss >> backs)) {
        Backtrace();
      } else {
        Backtrace(backs);
      }
      break;
    case 'q':
      std::cout << "Bye Bye\n";
      is_terminated = true;
      break;
    default:
      TomasuloSimulator::PrintUsage();
      break;
    }
  }
}
void TomasuloSimulator::SingleStep() {
  if (!is_finished_) {
    ++clocks_;
  }
  for (auto &inst : instructions_) {
    if (inst->write_time_ != -1) // writeback finished
      continue;
    // writeback not finished
    if (inst->exec_end_time_ != -1) { // execution finished
      switch (inst->instop_) {
      case InstOp::LOAD:
        if (register_status_[inst->rt_] == inst->station_) {
          registers_[inst->rt_] = inst->result_;
          register_status_[inst->rt_] = nullptr;
        }
      case InstOp::ADDD:
      case InstOp::SUBD:
      case InstOp::MULD:
      case InstOp::DIVD: {
        inst->write_time_ = clocks_;
        for (auto &load_station : loadstore_stations_) {
          if (load_station->Qj_ == inst->station_) {
            load_station->Vj_ = inst->result_;
            load_station->Qj_ = nullptr;
          }
          if (load_station->Qk_ == inst->station_) {
            load_station->Vk_ = inst->result_;
            load_station->Qk_ = nullptr;
          }
        }
        for (auto &reserv_station : reservation_stations_) {
          if (reserv_station->Qj_ == inst->station_) {
            reserv_station->Vj_ = inst->result_;
            reserv_station->Qj_ = nullptr;
            if (reserv_station->Qk_ == nullptr) {
              reserv_station->time_ = 0;
            }
          }
          if (reserv_station->Qk_ == inst->station_) {
            reserv_station->Vk_ = inst->result_;
            reserv_station->Qk_ = nullptr;
            if (reserv_station->Qj_ == nullptr) {
              reserv_station->time_ = 0;
            }
          }
        }
        if (inst->instop_ != InstOp::LOAD && register_status_[inst->rd_or_imm_] == inst->station_) {
          registers_[inst->rd_or_imm_] = inst->result_;
          register_status_[inst->rd_or_imm_] = nullptr;
        }
        inst->station_->ResetEmpty();
      } break;
      case InstOp::STORE:
        if (inst->station_->Qk_ == nullptr) {
          memory_[inst->result_] = inst->station_->Vk_;
          inst->station_->ResetEmpty();
          inst->write_time_ = clocks_;
        }
        break;
      case InstOp::NONE:
        break;
      }
    } else {                              // execution not finished
      if (inst->exec_begin_time_ != -1) { // execution started
        --inst->station_->time_;
        if (inst->station_->time_ == 0) {
          inst->exec_end_time_ = clocks_; // execution now just finished
          switch (inst->instop_) {
          case InstOp::LOAD:
          case InstOp::STORE:
            inst->result_ = "Mem[" + inst->station_->Address_ + "]";
            break;
          case InstOp::ADDD:
            inst->result_ = inst->station_->Vj_ + "+" + inst->station_->Vk_;
            break;
          case InstOp::SUBD:
            inst->result_ = inst->station_->Vj_ + "-" + inst->station_->Vk_;
            break;
          case InstOp::MULD:
            inst->result_ = inst->station_->Vj_ + "*" + inst->station_->Vk_;
            break;
          case InstOp::DIVD:
            inst->result_ = inst->station_->Vj_ + "/" + inst->station_->Vk_;
            break;
          case InstOp::NONE:
            break;
          }    
        }
      } else {                         // execution not start
        if (inst->issue_time_ != -1) { // has issued
          // allow to execute
          if (inst->station_->Qj_ == nullptr &&
              inst->station_->Qk_ == nullptr) {
            inst->exec_begin_time_ = clocks_ - ~inst->station_->time_;
            switch (inst->instop_) {
            case InstOp::LOAD:
            case InstOp::STORE: {
              inst->station_->Address_ =
                  inst->station_->Address_ + "+" + registers_[inst->rs_];
              inst->station_->time_ += loadstore_latency;
            } break;
            case InstOp::ADDD:
            case InstOp::SUBD:
              inst->station_->time_ += adddsubd_latency;
              break;
            case InstOp::MULD:
              inst->station_->time_ += multd_latency;
              break;
            case InstOp::DIVD:
              inst->station_->time_ += divd_latency;
              break;
            case InstOp::NONE:
              break;
            }
          }
        } else { // not issue, try issue
          switch (inst->instop_) {
          case InstOp::LOAD:
          case InstOp::STORE: {
            for (auto &load_station : loadstore_stations_) {
              if (load_station->busy_)
                continue;
              inst->issue_time_ = clocks_;
              inst->station_ = load_station;
              if (register_status_[inst->rs_] == nullptr) {
                load_station->Vj_ = registers_[inst->rs_];
                load_station->Qj_ = nullptr;
              } else {
                load_station->Vj_.clear();
                load_station->Qj_ = register_status_[inst->rs_];
              }
              load_station->busy_ = true;
              load_station->instop_ = inst->instop_;
              load_station->Address_ = inst->rd_or_imm_;
              if (inst->instop_ == InstOp::LOAD) {
                register_status_[inst->rt_] = load_station;
              } else { // Store
                if (register_status_[inst->rt_] == nullptr) {
                  load_station->Vk_ = registers_[inst->rt_];
                  load_station->Qk_ = nullptr;
                } else {
                  load_station->Vk_.clear();
                  load_station->Qk_ = register_status_[inst->rt_];
                }
              }
              break;
            }
          } break;
          case InstOp::ADDD:
          case InstOp::SUBD:
          case InstOp::MULD:
          case InstOp::DIVD: {
            for (auto &reservstation : reservation_stations_) {
              if (reservstation->busy_ == true ||
                  (reservstation->station_type_ == StationType::ADD &&
                   inst->instop_ != InstOp::ADDD &&
                   inst->instop_ != InstOp::SUBD) ||
                  ((reservstation->station_type_ == StationType::MULT &&
                    inst->instop_ != InstOp::MULD &&
                    inst->instop_ != InstOp::DIVD)))
                continue;
              inst->issue_time_ = clocks_;
              inst->station_ = reservstation;
              if (register_status_[inst->rs_] == nullptr) {
                reservstation->Vj_ = registers_[inst->rs_];
                reservstation->Qj_ = nullptr;
              } else {
                reservstation->Vj_.clear();
                reservstation->Qj_ = register_status_[inst->rs_];
              }
              if (register_status_[inst->rt_] == nullptr) {
                reservstation->Vk_ = registers_[inst->rt_];
                reservstation->Qk_ = nullptr;
              } else {
                reservstation->Vk_.clear();
                reservstation->Qk_ = register_status_[inst->rt_];
              }
              reservstation->busy_ = true;
              reservstation->instop_ = inst->instop_;
              register_status_[inst->rd_or_imm_] = reservstation;
              break;
            }
            break;
          }
          case InstOp::NONE: {
            std::cerr << "Something wrong with instruction issue!\n";
            abort();
          } break;
          }
          break;
        }
      }
    }
  }
}
void TomasuloSimulator::Step(size_t cycles) {
  std::cout << "Step " << cycles << '\n';
  for (size_t i = 0; i < cycles && !is_finished_; ++i) {
    SingleStep();
    PrintAllInfo();
  }
  if (is_finished_) {
    std::cout << "!!!All the instructions has been executed!!!\n";
  }
}
void TomasuloSimulator::RunToEnd() { std::cout << "Run to End \n"; }
void TomasuloSimulator::Backtrace(size_t cycles) {
  std::cout << "Backtrace " << cycles << '\n';
}
void TomasuloSimulator::PrintUsage() {
  std::cout
      << "Usage: \n"
      << "v [i | l | r | s | a] : "
         "display instructions status | load and reservation stations | "
         "registers result status | statistics | all information aforesaid\n"
      << "s [number(optional)] : step 1/number cycle(s)\n"
      << "r : run to the end\n"
      << "b [number] : backtrace to view the infomation of number cycle(s) "
         "ago\n"
      << "q : quit the simulator\n";
}
void TomasuloSimulator::PrintReservStation(const std::shared_ptr<Station> &reserv_station) {
  if (reserv_station->time_ != -1)
    std::cout << reserv_station->time_;
  std::cout << '\t';
  std::cout << reserv_station->name_ << "\t\t";
  std::cout << reserv_station->busy_ << '\t';
  if (reserv_station->busy_ == false) {
    std::cout << "\t\n";
    return;
  }
  std::cout << InstOpToStr(reserv_station->instop_) << '\t';
  std::cout << reserv_station->Vj_ << "\t\t" << reserv_station->Vk_ << "\t\t";
  if (reserv_station->Qj_ != nullptr) {
    std::cout << reserv_station->Qj_->name_;
  }
  std::cout << '\t';
  if (reserv_station->Qk_ != nullptr) {
    std::cout << reserv_station->Qk_->name_;
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