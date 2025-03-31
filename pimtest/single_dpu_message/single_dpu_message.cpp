#include <cstdio>
#include <random>

#include "../upmem-runtime/src/basic.hh"

int main()
{
  printf("version: 1.0.3\n");
  printf("entering single_dpu_message\n");
  dpu_init(16, "/home/weichu/my_gem5/gem5/src/pim/bin/1_dpus", ".");
  printf("dpu_init done\n");
  dpu_load("RED");
  printf("dpu_load done\n");
  dpu_launch(upmem_sim::dpu_launch_policy::DPU_ASYNCHRONOUS);
  while(dpu_check() == false) {
    //printf("Checking DPU status...\n");
  }
  printf("dpu_work done\n");

  return 0;
}
