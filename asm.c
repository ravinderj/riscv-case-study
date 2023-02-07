#include <stdio.h>

int main(int argc, char const *argv[])
{
    long src = 1;
    long dst;

    asm("mov %1, %0\n\t"
        "add %0, %0, %1"
        : "=r"(dst)
        : "r"(src));

    printf("%ld\n", dst);
    return 0;
}
