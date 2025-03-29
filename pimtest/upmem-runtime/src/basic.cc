#include<iostream>
#include <filesystem>
#include <atomic>

#include "basic.hh"
#include "../include/gem5/m5ops.h"

#include "byte_stream.h"
#include "pim/dpu_message.hh"
#include "pim/ringbuffer.hh"


RingBuffer<upmem_sim::SQ_entry*> *SQ;
RingBuffer<upmem_sim::CQ_entry*> *CQ;
std::atomic<size_t> CQ_head, SQ_tail,command_id;

//填充Submission Queue
void dpu_submit(upmem_sim::OPCODE opcode, std::any data){
  SQ->set(SQ_tail,new upmem_sim::SQ_entry(opcode,command_id.fetch_add(1),data));
  upmem_sim::Doorbells *doorbells = new upmem_sim::Doorbells(SQ_tail,CQ_head);
  upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_DOORBELL,data);
  printf("before m5_reserved1\n");
  m5_reserved1((void*)msg);
}
void dpu_init(int nr_tasklets, char *bindir, char *logdir){
  printf("entering dpu_init\n");
  char *argv[] = {
          (char *)"./src/uPIMulator",                                                 // argv[0]: 程序名
          (char *)"--benchmark", (char *)"RED",                                       // argv[1], argv[2]
          (char *)"--num_tasklets", (char *)std::to_string(nr_tasklets).c_str(),                                     // argv[3], argv[4]
          (char *)"--bindir", bindir, // argv[5], argv[6]
          (char *)"--logdir", logdir                                             // argv[7], argv[8]
  };
  int argc = sizeof(argv) / sizeof(argv[0]);
  SQ= new RingBuffer<upmem_sim::SQ_entry*>(16);
  CQ= new RingBuffer<upmem_sim::CQ_entry*>(16);
  CQ_head.store(0,std::memory_order_relaxed);
  SQ_tail.store(0,std::memory_order_relaxed);
  command_id.store(0,std::memory_order_relaxed);
  upmem_sim::init_argument *arg= new upmem_sim::init_argument(argv,argc,SQ,CQ);
  upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_INIT,arg);
  printf("before m5_reserved1\n");
  printf("msg type %d\n",msg->type);
  printf("msg addr: %ld\n",msg);
  m5_reserved1((void*)msg);



}


void dpu_load(std::string binary_path)
{
  printf("entering dpu_load\n");
  std::vector<std::string> binary_names={"atomic","iram","wram","mram"};
  std::vector<upmem_sim::encoder::ByteStream*> byte_streams;
  for (const auto& x:binary_names){
    std::string bin_filepath = binary_path + "/" + x + ".bin";
    if (std::filesystem::exists(bin_filepath)) {
      printf("file %s exists\n",bin_filepath.c_str());
      auto byte_stream = new upmem_sim::encoder::ByteStream(bin_filepath);
      byte_streams.push_back(byte_stream);
      printf("byte_stream size %ld\n",byte_stream->size());
    }
    else {
      printf("file %s does not exist\n",bin_filepath.c_str());
      exit(1);
    }
  }
  printf("after load byte_streams\n");
  upmem_sim::binaries_to_load * binaries= new upmem_sim::binaries_to_load(byte_streams[0],byte_streams[1],byte_streams[2],byte_streams[3]);

  dpu_submit(upmem_sim::LOAD, binaries);
}

void dpu_trans()
{
}

void dpu_launch()
{
}
