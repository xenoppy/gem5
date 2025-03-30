#include <cstdio>
#include <random>

#include "../upmem-runtime/src/basic.hh"

int main()
{
  printf("entering single_dpu_message\n");
  dpu_init(16, "/home/weichu/my_gem5/gem5/src/pim/bin/1_dpus", ".");
  printf("dpu_init done\n");
  dpu_load("RED");
  printf("dpu_load done\n");
  dpu_launch(upmem_sim::dpu_launch_policy::DPU_ASYNCHRONOUS);
  int b = 0, c = 2;
  while (1)
  {
    int a = 2;
    b = b + a;
    b = b - c;
  }
  return 0;
}
