#include "common.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ratio>
#include <vector>

void filter_golden(std::vector<float> input, size_t size_x, size_t size_y,
                     size_t filter_radius) {

  float *data = input.data();

  float *column = (float *)malloc(sizeof(float) * size_y);
  float *data_col =
      (float *)malloc(sizeof(float) * (size_y + 2 * filter_radius));
  float *data_row =
      (float *)malloc(sizeof(float) * (size_x + 2 * filter_radius));

  clock_t begin = clock();

  filter_gauss_2d_flt(data, column, data_row, data_col, size_x, size_y, 1,
                      filter_radius);

  clock_t end = clock();

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time elapsed GOLDEN: %f s\n", time_spent);
  free(column);
  free(data_col);
  free(data_row);
}

void filter_riscv(std::vector<float> input, size_t size_x, size_t size_y,
                    size_t filter_radius) {

  float *data = input.data();

  float *column = (float *)malloc(sizeof(float) * size_y);
  float *data_col =
      (float *)malloc(sizeof(float) * (size_y + 2 * filter_radius));
  float *data_row =
      (float *)malloc(sizeof(float) * (size_x + 2 * filter_radius));

  clock_t begin = clock();

  filter_gauss_2d_flt_riscv(data, column, data_row, data_col, size_x, size_y, 1,
                            filter_radius);

  clock_t end = clock();

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time elapsed RISC-V: %f s\n", time_spent);
  free(column);
  free(data_col);
  free(data_row);
}

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Usage: <executable> <X> <Y> <filter_size>\n");
    abort();
  }
  size_t x = atoi(argv[1]);
  size_t y = atoi(argv[2]);
  size_t filter_radius = atoi(argv[3]);

  if(( 2 * filter_radius + 1) > x || ( 2 * filter_radius + 1) > y){
    std::cerr << "Invalid filter radius because filter window exceeds dimensions" << std::endl;
    exit(1);
  } 

  std::vector<float> input = generateRandomData(x * y, 100, -50);
  std::vector<float> expected = input;

  filter_golden(expected, x, y, filter_radius);

  std::vector<float> actual_riscv = input;

  filter_riscv(actual_riscv, x, y, filter_radius);

  assert_cont_flt_array(expected.data(), actual_riscv.data(), x * y);

  printf("Assertions passed SSE\n");
}