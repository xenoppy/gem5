#include "simulator/cpu/init_thread.h"

#include <iostream>

namespace upmem_sim::simulator::cpu {

void InitThread::connect_rank(rank::Rank *rank) {
  assert(rank != nullptr);
  assert(rank_ == nullptr);

  rank_ = rank;
}

void InitThread::init() {
  dma_transfer_to_atomic();
  dma_transfer_to_iram();
  dma_transfer_to_wram();
  dma_transfer_to_mram();

  std::cout << "init completed..." << std::endl;
}

void InitThread::launch() {
  rank_->launch();

  std::cout << "launch completed..." << std::endl;
}

void InitThread::dma_transfer_to_atomic() {
  auto byte_stream = load_byte_stream("atomic");
  for (auto &dpu : rank_->dpus()) {
    dpu->dma()->transfer_to_atomic(util::ConfigLoader::atomic_offset(),
                                   byte_stream);
    if (byte_stream != nullptr) {
      auto rank_message = new rank::RankMessage(
          rank::RankMessage::WRITE, dpu->dpu_id(),
          sys_used_mram_end_pointer(),//TODO: should be atomic address
          byte_stream->size(), byte_stream);
      rank_->write(rank_message);
      rank_->insert_mem_message(rank_message);
    }
  }
  delete byte_stream;

  std::cout << "DMA to atomic completed..." << std::endl;
}

void InitThread::dma_transfer_to_iram() {
  auto byte_stream = load_byte_stream("iram");
  for (auto &dpu : rank_->dpus()) {
    dpu->dma()->transfer_to_iram(util::ConfigLoader::iram_offset(),
                                 byte_stream);

    if (byte_stream != nullptr) {
      auto rank_message = new rank::RankMessage(
          rank::RankMessage::WRITE, dpu->dpu_id(),
          sys_used_mram_end_pointer(),//TODO: should be iram address
          byte_stream->size(), byte_stream);
      rank_->write(rank_message);
      rank_->insert_mem_message(rank_message);
    }
  }
  delete byte_stream;
  std::cout << "DMA to IRAM completed..." << std::endl;
}

void InitThread::dma_transfer_to_wram() {
  auto byte_stream = load_byte_stream("wram");
  for (auto &dpu : rank_->dpus()) {
    dpu->dma()->transfer_to_wram(util::ConfigLoader::wram_offset(),
                                 byte_stream);
    if (byte_stream != nullptr) {
      auto rank_message = new rank::RankMessage(
          rank::RankMessage::WRITE, dpu->dpu_id(),
          sys_used_mram_end_pointer(),//TODO: should be wram address
          byte_stream->size(), byte_stream);
      rank_->write(rank_message);
      rank_->insert_mem_message(rank_message);
    }
  }
  delete byte_stream;

  std::cout << "DMA to WRAM completed..." << std::endl;
}

void InitThread::dma_transfer_to_mram() {
  auto byte_stream = load_byte_stream("mram");
  for (auto &dpu : rank_->dpus()) {
    dpu->dma()->transfer_to_mram(util::ConfigLoader::mram_offset(),
                                 byte_stream);

    if (byte_stream != nullptr) {
      auto rank_message = new rank::RankMessage(
          rank::RankMessage::WRITE, dpu->dpu_id(),
          sys_used_mram_end_pointer(),//TODO: should be exact mram address
          byte_stream->size(), byte_stream);
      rank_->write(rank_message);
      rank_->insert_mem_message(rank_message);
    }
  }
  delete byte_stream;

  std::cout << "DMA to MRAM completed..." << std::endl;
}

}  // namespace upmem_sim::simulator::cpu
