#include <cassert>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <riscv_vector.h>

class WallClock {
  std::chrono::high_resolution_clock::time_point begin;

public:
  WallClock() { begin = std::chrono::high_resolution_clock::now(); }
  void tick() { begin = std::chrono::high_resolution_clock::now(); }
  void reset() { begin = std::chrono::high_resolution_clock::now(); }
  double elapsedTime() {
    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
        .count();
  }
};

void assert_float(float expected, float actual, std::string str) {
  float diff = fabs(expected - actual);
  float loss = (diff / fabs(expected)) * 100;

  float tolerance = 0.05; // percentage
  if (loss > tolerance) {
    std::cerr << str << "ASSERTION ERROR: expected: " << expected
              << " actual: " << actual << " loss: " << loss << "% "
              << std::endl;
    assert(loss < tolerance);
  }
}

void golden(float *arr, size_t N, float &min, float &max) {
  min = FLT_MAX;
  max = FLT_MIN;
  for (size_t i = 0; i < N; i++) {
    if (min > arr[i]) {
      min = arr[i];
    }
    if (max < arr[i]) {
      max = arr[i];
    }
  }
}

void simd_riscv(float *arr, size_t N, float &min, float &max) {
  // assert(N < (size_t)INT_MAX);
  size_t maxvl = vsetvl_e32m1(N);
  vfloat32m1_t arr_r = vle32_v_f32m1(arr, maxvl);
  vfloat32m1_t min_r = arr_r;
  vfloat32m1_t max_r = arr_r;
  arr += maxvl;
  N -= maxvl;

  for (size_t vl; N > 0; arr += vl, N -= vl) {
    vl = vsetvl_e32m1(N);
    arr_r = vle32_v_f32m1(arr, vl);
    min_r = vfmin_vv_f32m1(arr_r, min_r, vl);
    max_r = vfmax_vv_f32m1(arr_r, max_r, vl);
  }

  float tmp_min[maxvl];
  float tmp_max[maxvl];
  vse32_v_f32m1(tmp_min, min_r, maxvl);
  vse32_v_f32m1(tmp_max, max_r, maxvl);
  min = tmp_min[0];
  max = tmp_max[0];

  for (size_t i = 1; i < maxvl; i++) {
    if (min > tmp_min[i]) {
      min = tmp_min[i];
    }
    if (max < tmp_max[i]) {
      max = tmp_max[i];
    }
  }
}


void simd_riscv_m2(float *arr, size_t N, float &min, float &max) {
  // assert(N < (size_t)INT_MAX);
  size_t maxvl = vsetvl_e32m2(N);
  vfloat32m2_t arr_r = vle32_v_f32m2(arr, maxvl);
  vfloat32m2_t min_r = arr_r;
  vfloat32m2_t max_r = arr_r;
  arr += maxvl;
  N -= maxvl;

  for (size_t vl; N > 0; arr += vl, N -= vl) {
    vl = vsetvl_e32m2(N);
    arr_r = vle32_v_f32m2(arr, vl);
    min_r = vfmin_vv_f32m2(arr_r, min_r, vl);
    max_r = vfmax_vv_f32m2(arr_r, max_r, vl);
  }

  float tmp_min[maxvl];
  float tmp_max[maxvl];
  vse32_v_f32m2(tmp_min, min_r, maxvl);
  vse32_v_f32m2(tmp_max, max_r, maxvl);
  min = tmp_min[0];
  max = tmp_max[0];

  for (size_t i = 1; i < maxvl; i++) {
    if (min > tmp_min[i]) {
      min = tmp_min[i];
    }
    if (max < tmp_max[i]) {
      max = tmp_max[i];
    }
  }
}


void simd_riscv_m4(float *arr, size_t N, float &min, float &max) {
  // assert(N < (size_t)INT_MAX);
  size_t maxvl = vsetvl_e32m4(N);
  vfloat32m4_t arr_r = vle32_v_f32m4(arr, maxvl);
  vfloat32m4_t min_r = arr_r;
  vfloat32m4_t max_r = arr_r;
  arr += maxvl;
  N -= maxvl;

  for (size_t vl; N > 0; arr += vl, N -= vl) {
    vl = vsetvl_e32m4(N);
    arr_r = vle32_v_f32m4(arr, vl);
    min_r = vfmin_vv_f32m4(arr_r, min_r, vl);
    max_r = vfmax_vv_f32m4(arr_r, max_r, vl);
  }

  float tmp_min[maxvl];
  float tmp_max[maxvl];
  vse32_v_f32m4(tmp_min, min_r, maxvl);
  vse32_v_f32m4(tmp_max, max_r, maxvl);
  min = tmp_min[0];
  max = tmp_max[0];

  for (size_t i = 1; i < maxvl; i++) {
    if (min > tmp_min[i]) {
      min = tmp_min[i];
    }
    if (max < tmp_max[i]) {
      max = tmp_max[i];
    }
  }
}

void simd_riscv_m8(float *arr, size_t N, float &min, float &max) {
  // assert(N < (size_t)INT_MAX);
  size_t maxvl = vsetvl_e32m8(N);
  vfloat32m8_t arr_r = vle32_v_f32m8(arr, maxvl);
  vfloat32m8_t min_r = arr_r;
  vfloat32m8_t max_r = arr_r;
  arr += maxvl;
  N -= maxvl;

  for (size_t vl; N > 0; arr += vl, N -= vl) {
    vl = vsetvl_e32m8(N);
    arr_r = vle32_v_f32m8(arr, vl);
    min_r = vfmin_vv_f32m8(arr_r, min_r, vl);
    max_r = vfmax_vv_f32m8(arr_r, max_r, vl);
  }

  float tmp_min[maxvl];
  float tmp_max[maxvl];
  vse32_v_f32m8(tmp_min, min_r, maxvl);
  vse32_v_f32m8(tmp_max, max_r, maxvl);
  min = tmp_min[0];
  max = tmp_max[0];

  for (size_t i = 1; i < maxvl; i++) {
    if (min > tmp_min[i]) {
      min = tmp_min[i];
    }
    if (max < tmp_max[i]) {
      max = tmp_max[i];
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << " usage: <executable> <size of array>" << std::endl;
    return 1;
  }
  size_t x = atoi(argv[1]);
  size_t N = x * x;
  float *arr = new float[N];
  time_t seed = time(NULL);
  // time_t seed = 1668773523;
  std::cout << "Seed: " << seed << std::endl;
  srand(seed);

  float range = 100.0;
  float offset = -50.0;
  for (size_t i = 0; i < N; i++) {
    arr[i] = offset + range * (rand() / (float)RAND_MAX);
  }

  std::cout << "Matrix dim: " << x << std::endl;

  float minExpected = FLT_MAX, maxExpected = FLT_MIN;
  {
    WallClock t;

    golden(arr, N, minExpected, maxExpected);

    std::cout << "Elapsed time golden: " << t.elapsedTime() << " us"
              << std::endl;
  }
  printf("Golden min: %f ", minExpected);
  printf("Golden max: %f \n", maxExpected);

  {
    float minActual = FLT_MAX, maxActual = FLT_MIN;
    WallClock t;

    simd_riscv(arr, N, minActual, maxActual);

    std::cout << "Elapsed time RISCV-V: " << t.elapsedTime() << " us"
              << std::endl;

    assert_float(maxExpected, maxActual, "maxRISC-V");
    assert_float(minExpected, minActual, "minRISC-V");
  }

  {
    float minActual = FLT_MAX, maxActual = FLT_MIN;
    WallClock t;

    simd_riscv_m2(arr, N, minActual, maxActual);

    std::cout << "Elapsed time RISCV-V m2 : " << t.elapsedTime() << " us"
              << std::endl;

    assert_float(maxExpected, maxActual, "maxRISC-V-m2");
    assert_float(minExpected, minActual, "minRISC-V-m2");
  }

  {
    float minActual = FLT_MAX, maxActual = FLT_MIN;
    WallClock t;

    simd_riscv_m4(arr, N, minActual, maxActual);

    std::cout << "Elapsed time RISCV-V m4 : " << t.elapsedTime() << " us"
              << std::endl;

    assert_float(maxExpected, maxActual, "maxRISC-V-m4");
    assert_float(minExpected, minActual, "minRISC-V-m4");
  }

  {
    float minActual = FLT_MAX, maxActual = FLT_MIN;
    WallClock t;

    simd_riscv_m8(arr, N, minActual, maxActual);

    std::cout << "Elapsed time RISCV-V m8 : " << t.elapsedTime() << " us"
              << std::endl;

    assert_float(maxExpected, maxActual, "maxRISC-V-m8");
    assert_float(minExpected, minActual, "minRISC-V-m8");
  }

}