# RISC-V

## TODO:
- Try out RISC-V GNU toolchain
- Check if time.h works for benchmarking
- Use RDCYCLE to benchmark code

### Install risc-v toolchain on mac
- brew tap riscv-software-src/riscv
- brew install riscv-tools

### To compile code and run it in riscv simulator
- riscv64-unknown-elf-gcc
- spike pk <riscv-executable>
- spike --isa rv64gcv pk <riscv-executable> (for vector instruction code)

Running code in RISCV simulator takes more time than running it on machine directly.

### Benchmarking with c time library
When using c time library to benchmark,
execution time remains constant for the same input.

### Benchmarking with RDCYCLE instruction
When using RDCYCLE instruction to benchmark,
cycle-counts remains constant for the same input.


## theories
- Diff between program execution time and
  simulator overhead time
  (assuming the total time taken is more
   than the program execution time)
Tm = time to execute on machine
Ts = time to execute on simulator
  is it as simple as diff between Ts - Tm.

simulator takes more time
how to benchmark
/ try with code
- RDCYCLE
- RISCV time benchmark always gives same
  elapsed time for a given input size run
  on a program (is it reliable?)
- is the time calculated based on
  cycle count
  e.g. 20000000 and 20000 microsec
  and  2000000 and 2000 microsec
- based on the above calculations and observations
  it seems the clock speed is 1gHz (assumption: time
  is calculated based on clock cycle count)


## Register Size

The register size is 128 bit

e32m1 = single register, containing 4 element of 32 bit size, vl = 4
e32m2 = 2 registers, each containing 4 element of 32 bit size, vl = 8
e64m1 = single register, containing 2 element of 64 bit size, vl = 2
e64m2 = 2 registers, each containing 2 element of 64 bit size, vl = 4


## Benchmarking min-max case study using RISC-V
Seed: 0
Matrix dim: 1000
Elapsed time golden: 24006 us
Golden min: -49.999943 Golden max: 49.999832
Elapsed time RISCV-V: 35501 us
Elapsed time RISCV-V m2 : 18001 us
Elapsed time RISCV-V m4 : 9002 us
Elapsed time RISCV-V m8 : 4503 us
