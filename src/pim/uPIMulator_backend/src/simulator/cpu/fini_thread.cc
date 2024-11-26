#include "simulator/cpu/fini_thread.h"

namespace upmem_sim::simulator::cpu
{

  void FiniThread::connect_rank(rank::Rank *rank)
  {
    assert(rank != nullptr);
    assert(rank_ == nullptr);

    rank_ = rank;
  }

  /**
   * @brief Executes a cycle operation for the FiniThread.
   *
   * This function iterates over all DPUs (Data Processing Units) in the rank and
   * checks each thread managed by the DPU's scheduler. If a thread's program counter
   * register (PC) matches the system end pointer and the thread is in the SLEEP state,
   * the thread is shut down by the scheduler.
   */
  void FiniThread::cycle()
  {
    for (auto &dpu : rank_->dpus())
    {
      for (auto &thread : dpu->scheduler()->threads())
      {
        if (thread->reg_file()->read_pc_reg() == sys_end_pointer() and
            thread->state() == dpu::Thread::SLEEP)
        {
          std::cout << "MYLOG: fini_thread shutup a dpu" << std::endl;
          dpu->scheduler()->shutdown(thread->id());
        }
      }
    }
  }

} // namespace upmem_sim::simulator::cpu
