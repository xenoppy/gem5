#ifndef BASIC_HH
#define BASIC_HH


#include "pim/dpu_message.hh"

void dpu_init(int nr_tasklets, char *bindir, char *logdir);
void dpu_load(std::string binary_path);
void dpu_trans();
void dpu_launch(upmem_sim::dpu_launch_policy policy);
bool dpu_check();



void dpu_init(int nr_tasklets, char *bindir, char *logdir);
void dpu_load(std::string binary_path);

#endif // BASIC_HH
