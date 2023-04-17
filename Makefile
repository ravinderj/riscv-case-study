ifndef RISCV
$(error RISCV is not set. Please set it to the root dir of RISCV installation.)
endif

CC=${RISCV}/bin/riscv64-unknown-linux-gnu-gcc

CXX=${RISCV}/bin/riscv64-unknown-linux-gnu-g++

all: build/abs.out \
	build/riscv_omp.out \
	build/riscv_v_minMaxBench.out \
	build/riscv_v_GaussianFilter.out

build/%.out: src/%.c dir
	$(CC) -static -march=rv64gcv -I include -o $@ $< $(OPT)

build/%.out: src/%.cpp dir
	$(CXX) -static -march=rv64gcv -I include -o $@ $< $(OPT)

dir:
	mkdir -p build

.PHONY: dir clean

clean:
	rm -rf build/*