#ifndef BASIC_HH
#define BASIC_HH

void dpu_init(int nr_tasklets, char *bindir, char *logdir);
void dpu_load(std::string binary_path);
void dpu_trans();
void dpu_launch();

#endif // BASIC_HH
