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
  for(size_t i = 0; i < args.size(); ++i) {
    // Print the contents of each argument for debugging
    std::cout<<(char*)argv_ptr[i]->data<<std::endl; // Print the data for each message_data
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
  // //just to load the binary files, not used, maybe count for memory reading for gem5 simulated memory
  // std::vector<std::string> binary_names={"atomic","iram","wram","mram"};
  // std::vector<upmem_sim::encoder::ByteStream*> byte_streams;
  // for (const auto& x:binary_names){
  //   std::string bin_filepath = binary_path + "/" + x + ".bin";
  //   if (std::filesystem::exists(bin_filepath)) {
  //     printf("file %s exists\n",bin_filepath.c_str());
  //     auto byte_stream = new upmem_sim::encoder::ByteStream(bin_filepath);
  //     byte_streams.push_back(byte_stream);
  //     printf("byte_stream size %ld\n",byte_stream->size());
  //   }
  //   else {
  //     printf("file %s does not exist\n",bin_filepath.c_str());
  //     exit(1);
  //   }
  // }
  // printf("after load byte_streams\n");
  // size_t argc=1;
  // char *argv[;
  // upmem_sim::Dpu_message *msg = new upmem_sim::Dpu_message(upmem_sim::DPU_INIT,argc,(void**)argv);
  // printf("before m5_reserved1\n");
  // printf("msg type %d\n",msg->type);
  // printf("msg addr: %ld\n",msg);
  // m5_reserved1((void*)msg);

}

void dpu_trans()
{
}

void dpu_launch()
{
}
