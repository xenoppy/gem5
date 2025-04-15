#include <cstdio>
#include <random>

#include "../upmem-runtime/src/basic.hh"

int main()
{
  printf("version: 1.0.3\n");
  printf("entering single_dpu_message\n");
  dpu_init(16, "/home/weichu/my_gem5/gem5/src/pim/bin/1_dpus", ".");
  printf("cpp: dpu_init done\n");
  dpu_load("RED");
  printf("cpp: dpu_load done\n");
  dpu_trans("RED");
  printf("cpp: dpu_trans done\n");
  dpu_launch(upmem_sim::dpu_launch_policy::DPU_ASYNCHRONOUS);
  while(!dpu_check_all()){
  }
  printf("cpp: dpu_work done\n");

  return 0;
}
