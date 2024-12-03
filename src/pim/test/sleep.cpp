#include <unistd.h>
int main(int argc, char **argv)
{
    int b = 0, c = 2;
    while (1)
    {
        int a = 2;
        b = b + a;
        b = b - c;
    }
    return 0;
}