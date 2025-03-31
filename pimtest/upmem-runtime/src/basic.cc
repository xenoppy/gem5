#include<iostream>
#include <filesystem>
#include <atomic>

#include "basic.hh"
#include "../include/gem5/m5ops.h"

#include "byte_stream.h"



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
  printf("init ready to call m5_op, msg type %d\n",msg->type);
  size_t ret = m5_reserved1((void*)msg); // Call m5_op to send the message to the simulator

  return;


}


void dpu_load(std::string binary_path)
{
  printf("entering dpu_load\n");
  size_t argc=1;
  upmem_sim::message_data** argv_ptr = new upmem_sim::message_data*[argc];
  argv_ptr[0] = new upmem_sim::message_data(binary_path.size()+1,binary_path.c_str());
  upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_LOAD,argc,argv_ptr);
  printf("load ready to call m5_op, msg type %d\n",msg->type);
  size_t ret = m5_reserved1((void*)msg); // Call m5_op to send the message to the simulator

}

void dpu_trans()
{
}

void dpu_launch(upmem_sim::dpu_launch_policy policy)
{
  size_t argc=1;
  upmem_sim::message_data** argv_ptr = new upmem_sim::message_data*[argc];
  argv_ptr[0] = new upmem_sim::message_data(sizeof(upmem_sim::dpu_launch_policy),&policy);
  upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_LAUNCH,argc,argv_ptr);
  printf("init ready to call m5_op, msg type %d\n",msg->type);
  size_t ret = m5_reserved1((void*)msg); // Call m5_op to send the message to the simulator

}

bool dpu_check(){

  upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_CHECK_FINISHED,0,nullptr);
  //printf("init ready to call m5_op, msg type %d\n",msg->type);
  size_t  ret=m5_reserved1((void*)msg);
  //std::cout<<"dpu_check: m5_reserved1 returned: " << ret << std::endl;
  if(ret){
    printf("dpu_check: DPU_CHECK_FINISHED returned true\n");
  }
  //printf("dpu_wait_finished: after m5_reserved1\n");
  return ret;
}
