export RISCV=~/Documents/opt/RISCV

# Steps to install gnu-toolchain with rvv-next branch for Vector extension

git clone https://github.com/riscv/riscv-gnu-toolchain -b rvv-next

cd riscv-gnu-toolchain

git submodule update --init --recursive riscv-gcc riscv-binutils newlib

./configure --prefix=$RISCV --with-cmodel=medany enable-multilib

make

# Steps to install isa-sim

git clone https://github.com/riscv/riscv-isa-sim.git

cd risc-isa-sim

mkdir build

cd build

../configure --prefix=$RISCV --with-boost=yes --with-prefix=riscv64-unknown-linux-gnu

make && make install


# Steps to install pk

git clone https://github.com/riscv-software-src/riscv-pk.git

cd riscv-pk

export CC=$RISCV/bin/riscv64-unknown-linux-gnu-gcc

mkdir build

cd build

# not clear about which payload file to use, but it works
../configure --prefix=$RISCV --host=riscv64-unknown-linux-gnu --with-payload=../dummy_payload/dummy_payload.lds

make install


# to compile vector instruction code
# https://github.com/riscv-collab/riscv-gnu-toolchain/issues/460

# $RISCV/bin/riscv64-unknown-linux-gnu-g++ -static -march=rv64gcv -o <output_executable> <source_code>

# $RISCV/bin/spike --isa rv64gcv pk <executable>