#include "simulator/system.h"

namespace upmem_sim::simulator
{

  System::System(util::ArgumentParser *argument_parser)
      : cpu_(new cpu::CPU(argument_parser)),
        rank_(new rank::Rank(argument_parser)),
        execuion_(0),
        stat_factory_(new util::StatFactory("System"))
  {
    benchmark = argument_parser->get_string_parameter("benchmark");

    cpu_->connect_rank(rank_);
  }

  System::~System()
  {
    delete cpu_;
    delete rank_;

    delete stat_factory_;
  }

  util::StatFactory *System::stat_factory()
  {
    auto stat_factory = new util::StatFactory("");

    util::StatFactory *rank_stat_factory = rank_->stat_factory();

    stat_factory->merge(stat_factory_);
    stat_factory->merge(rank_stat_factory);

    delete rank_stat_factory;

    return stat_factory;
  }
  // 看上去是把bin扔到dpu的IRAM，DRAM，WRAM，MRAM里面
  void System::init()
  {
    cpu_->init();
    cpu_->sched(execuion_);
    cpu_->launch();
  }
  void System::cpu_cycle()
  {
    cpu_->cycle();

    if (is_zombie())
    {
      cpu_->check(execuion_);
      execuion_ += 1;

      if (not is_finished())
      {
        if (benchmark == "TRNS")
        {
          cpu_->init();
        }
        cpu_->sched(execuion_);
        cpu_->launch();
      }
    }
  }
  void System::rank_cycle()
  {
    rank_->cycle();
  }
  void System::cycle()
  {
    // 遍历所有dpu看看有没有完成执行的dpu，需要被shutdown成zombie
    cpu_->cycle();
    rank_->cycle();
  }

} // namespace upmem_sim::simulator
