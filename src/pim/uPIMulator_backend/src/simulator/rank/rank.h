#ifndef UPMEM_SIM_SIMULATOR_RANK_RANK_H_
#define UPMEM_SIM_SIMULATOR_RANK_RANK_H_

#include <vector>

#include "main.h"
#include "simulator/basic/timer_queue.h"
#include "simulator/dpu/dpu.h"
#include "simulator/rank/rank_message.h"

namespace upmem_sim::simulator::rank {

class Rank {
 public:
  explicit Rank(util::ArgumentParser *argument_parser);
  ~Rank();

  util::StatFactory *stat_factory();

  std::vector<dpu::DPU *> dpus() { return dpus_; }

  void launch();
  bool is_zombie();

  void read(RankMessage *rank_message);
  void write(RankMessage *rank_message);
  void insert_mem_message(rank::RankMessage *rank_message) {
    mem_messages_.insert(rank_message);
  }

  /// @brief check if all mem messages are addressed
  /// @return true: all mem messages are finished, false: not all mem messages are finished
  bool check_mem_messages_finished() {
    //printf("entering check_mem_messages_finished\n");
    for (auto &mem_message : mem_messages_) {
      if (not mem_message->ack()) {
        return false;
      }
    }


    for (auto &mem_message : mem_messages_) {
      printf("all mem_message are acked\n");
      delete mem_message;
    }
    mem_messages_.clear();

    return true;
  }

  void cycle();

 protected:
  void service_sequence_q();

 private:
  Address read_bandwidth_;
  Address write_bandwidth_;
  std::set<rank::RankMessage*> mem_messages_;

  std::vector<dpu::DPU *> dpus_;
  std::vector<basic::TimerQueue<RankMessage>*> communication_qs_;

  util::StatFactory *stat_factory_;
};

}  // namespace upmem_sim::simulator::rank

#endif
