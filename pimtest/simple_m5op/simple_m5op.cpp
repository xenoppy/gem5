#include <cstdio>
#include <random>

#include "../include/gem5/m5ops.h"

int main()
{
  printf("\ntest begin\n");
  m5_reserved1();
  int b = 0, c = 2;
  while (1)
  {
    int a = 2;
    b = b + a;
    b = b - c;
  }
  return 0;
}
