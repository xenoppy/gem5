#include <cstdio>
#include <random>

#include "../upmem-runtime/src/basic.hh"

int main()
{
  printf("version: 1.0.3\n");
  printf("entering single_dpu_message\n");
  dpu_init_asynchronous(16, "/home/weichu/my_gem5/gem5/src/pim/bin/1_dpus", ".");
  printf("dpu_init done\n");
  dpu_load_asynchronous("RED");
  printf("dpu_load done\n");
  dpu_launch_asynchronous(upmem_sim::dpu_launch_policy::DPU_ASYNCHRONOUS);
  printf("dpu_work done\n");
  int b=0,c=0;
  while (1)
  {
    int a = 2;
    b = b + a;
    b = b - c;
  }

  return 0;
}
