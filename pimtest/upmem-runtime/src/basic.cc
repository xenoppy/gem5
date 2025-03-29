#include<iostream>
#include <filesystem>
#include <atomic>

#include "basic.hh"
#include "../include/gem5/m5ops.h"

#include "byte_stream.h"
#include "pim/dpu_message.hh"



void dpu_init(int nr_tasklets, char *bindir, char *logdir){
  printf("entering dpu_init\n");
  std::vector<std::string> args={
    "./src/PIMulator",   // argv[0]: simulator name
    "--num_tasklets",  std::to_string(nr_tasklets),
    (char *)"--bindir", bindir,
    (char *)"--logdir", logdir
  };
  upmem_sim::message_data** argv_ptr = new upmem_sim::message_data*[args.size()];
  for(size_t i = 0; i < args.size(); ++i) {
    // Convert std::string to char* for the DPU message
    argv_ptr[i] = new upmem_sim::message_data(args[i].size()+1,args[i].c_str());
  }

  upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_INIT,args.size(),argv_ptr);
  printf("before m5_reserved1\n");
  printf("msg type %d\n",msg->type);
  printf("msg addr: %ld\n",msg);
  m5_reserved1(msg);
  return;


}


void dpu_load(std::string binary_path)
{
  printf("entering dpu_load\n");
  size_t argc=1;
  const char *argv=binary_path.c_str(); // Convert std::string to char* for the DPU message
  upmem_sim::message_data** argv_ptr = new upmem_sim::message_data*[argc];
  argv_ptr[0] = new upmem_sim::message_data(binary_path.size()+1,binary_path.c_str());
  upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_LOAD,argc,argv_ptr);
  printf("before m5_reserved1\n");
  printf("msg type %d\n",msg->type);
  printf("msg addr: %ld\n",msg);
  m5_reserved1((void*)msg);

}

void dpu_trans()
{
}

void dpu_launch()
{
}
