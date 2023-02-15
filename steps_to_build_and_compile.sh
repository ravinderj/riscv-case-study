#!/usr/bin/env bash

set -x

# This script is designed to be used on alma-linux machines

export RISCV=~/opt/RISCV;

export RISCV_SOURCE=~/Documents/opensource/RISCV_SOURCE

mkdir $RISCV_SOURCE
cd $RISCV_SOURCE

echo "Starting : build riscv-gnu-toolchain with rvv-next branch"
# Steps to install gnu-toolchain with rvv-next branch for Vector extension

echo "Installing dependencies for riscv-gnu-toolchain"
sudo yum install autoconf automake python3 libmpc-devel mpfr-devel gmp-devel gawk  bison flex texinfo patchutils gcc gcc-c++ zlib-devel expat-devel

git clone https://github.com/riscv/riscv-gnu-toolchain -b rvv-next

cd riscv-gnu-toolchain

git submodule update --init --recursive riscv-gcc riscv-binutils newlib

./configure --prefix=$RISCV --with-cmodel=medany enable-multilib

make

echo "Finished : build riscv-gnu-toolchain"

cd $RISCV_SOURCE

# Steps to install isa-sim
echo "Starting : build riscv-isa-sim"
echo "Installing dependencies for riscv-isa-sim"
sudo yum install dtc boost

git clone https://github.com/riscv/riscv-isa-sim.git

cd riscv-isa-sim

mkdir build

cd build

../configure --prefix=$RISCV --with-boost=yes --with-prefix=riscv64-unknown-linux-gnu

make && make install

echo "Finished : build riscv-isa-sim"

cd $RISCV_SOURCE

# Steps to install pk
echo "Starting : build riscv-pk"

git clone https://github.com/riscv-software-src/riscv-pk.git

cd riscv-pk

export CC=$RISCV/bin/riscv64-unknown-linux-gnu-gcc

mkdir build

cd build

# not clear about which payload file to use, but it works
../configure --prefix=$RISCV --host=riscv64-unknown-linux-gnu --with-payload=../dummy_payload/dummy_payload.lds

make install

echo "Finished : build riscv-pk"

cd $RISCV_SOURCE

echo "export RISCV=$RISCV" >> ~/.bashrc
echo "export PATH=\$PATH:\$RISCV/bin" >> ~/.bashrc
echo "export RISCV_SOURCE=$RISCV_SOURCE" >> ~/.bashrc

# to compile vector instruction code
# https://github.com/riscv-collab/riscv-gnu-toolchain/issues/460

# $RISCV/bin/riscv64-unknown-linux-gnu-g++ -static -march=rv64gcv -o <output_executable> <source_code>

# $RISCV/bin/spike --isa rv64gcv pk <executable>