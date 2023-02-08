#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void genInput(int size, int * in) {
    for (int i = 0; i < size; i++)
    {
        int sign = (rand() % 2) ? -1 : 1;
        in[i] = sign * rand() % 1000;
    }
}

static inline uint32_t rdcyc() {
  uint32_t val;
  asm volatile ("rdcycle %0 ;\n":"=r" (val) ::);
  return val;
}

int main(int argc, char const *argv[])
{
    struct timeval start, end;
    uint32_t t_beg, t_end;
    char *host = "HOST";
    #ifdef RISCV
    t_beg     = rdcyc();
    host = "RISC-V";
    #endif
    gettimeofday(&start, NULL);

// ===============================
    int size = atoi(argv[1]);
    printf("%s,%d,", host, size);

    int *numbers = calloc(size, sizeof(int));
    if (numbers == NULL) {
        printf("Memory not allocate\n");
        exit(1);
    }
    genInput(size, numbers);

    for (size_t i = 0; i < size; i++)
    {
        numbers[i] = abs(numbers[i]);
    }

    free(numbers);
// ===============================

    gettimeofday(&end, NULL);
    printf("%d,", end.tv_usec - start.tv_usec);
    #ifdef RISCV
    t_end     = rdcyc();
    printf("%d\n", t_end - t_beg);
    #endif
    return 0;
}
