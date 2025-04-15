#ifndef UPMEM_SIM_SIMULATOR_SYSTEM_H_
#define UPMEM_SIM_SIMULATOR_SYSTEM_H_

#include "simulator/cpu/cpu.h"
#include "simulator/dpu/dpu.h"
#include "simulator/rank/rank.h"
#include "../../dpu_message.hh"

namespace upmem_sim::simulator
{

  class System
  {
  public:
    explicit System(util::ArgumentParser *argument_parser);
    ~System();
    bool is_zombie() { return rank_->is_zombie(); }
    util::ArgumentParser *get_argument_parser() {
      return argument_parser_;
    }
    util::StatFactory *stat_factory();
    dpu_launch_policy launch_policy;

    bool is_finished() { return execuion_ == cpu_->num_executions(); }
    bool is_benchmark_set;

    void set_benchmark(std::string benchmark_name) {
      benchmark = benchmark_name;
      cpu_->set_benchmark(benchmark_name);
      is_benchmark_set= true;


    }
    void load() { cpu_->init(); }
    void sched() { cpu_->sched(execuion_); }
    void launch() { cpu_->launch(); }
    bool check_mem_transport_finished(){
      return rank_->check_mem_messages_finished();
    }
    void init();
    void fini() { cpu_->fini(); }
    void cycle();
    void cpu_cycle();
    void dpu_check_cycle();
    void cpu_check_cycle();
    void rank_cycle();

  protected:
  private:
    util::ArgumentParser *argument_parser_;
    cpu::CPU *cpu_;
    rank::Rank *rank_;

    int execuion_;

    util::StatFactory *stat_factory_;

    // Tae
    std::string benchmark;
  };

} // namespace upmem_sim::simulator

#endif
