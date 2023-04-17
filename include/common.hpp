#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <riscv_vector.h>

#define FILTER_NAN(x) ((x) == (x) ? (x) : 0)

std::vector<float> generateRandomData(const size_t size, float range,
                                      float offset) {
  std::vector<float> arr(size);

  time_t seed = time(0);
  srand(seed);

  for (size_t i = 0; i < arr.size(); i++) {
    arr[i] = offset + range * (rand() / (float)RAND_MAX);
  }

  return arr;
}

void assert_cont_flt_array(float *expected, float *actual, size_t size) {
  for (size_t y = 0; y < size; y++) {
    float expected_value = expected[y];
    float actual_value = actual[y];

    float diff = fabs(expected_value - actual_value);
    float loss = (diff / fabs(expected_value)) * 100;

    float tolerance = 0.05; // percentage
    if (loss > tolerance) {
      std::cerr << "ASSERTION ERROR: expected: " << expected_value
                << " actual: " << actual_value << " loss: " << loss << "% "
                << std::endl;
      exit(1);
    }
  }
}

void filter_boxcar_1d_flt(float *data, float *data_copy, const size_t size,
                          const size_t filter_radius) {
  // Define filter size
  const size_t filter_size = 2 * filter_radius + 1;
  const float inv_filter_size = 1.0 / filter_size;
  size_t i;

  // Make copy of data, taking care of NaN
  for (i = size; i--;)
    data_copy[filter_radius + i] = FILTER_NAN(data[i]);

  // Fill overlap regions with 0
  for (i = filter_radius; i--;)
    data_copy[i] = data_copy[size + filter_radius + i] = 0.0;

  // Apply boxcar filter to last data point
  data[size - 1] = 0.0;
  for (i = filter_size; i--;)
    data[size - 1] += data_copy[size + i - 1];
  data[size - 1] *= inv_filter_size;

  // Recursively apply boxcar filter to all previous data points
  for (i = size - 1; i--;)
    data[i] = data[i + 1] +
              (data_copy[i] - data_copy[filter_size + i]) * inv_filter_size;

  return;
}

/* Code taken from SoFiA https://github.com/SoFiA-Admin/SoFiA-2.git */

void filter_gauss_2d_flt(float *data, float *data_copy, float *data_row,
                         float *data_col, const size_t size_x,
                         const size_t size_y, const size_t n_iter,
                         const size_t filter_radius) {
  // Set up a few variables
  const size_t size_xy = size_x * size_y;
  float *ptr = data + size_xy;
  float *ptr2;

  // Run row filter (along x-axis)
  // This is straightforward, as the data are contiguous in x.
  while (ptr > data) {
    ptr -= size_x;
    // for (size_t i = n_iter; i--;)
    filter_boxcar_1d_flt(ptr, data_row, size_x, filter_radius);
  }

  // Run column filter (along y-axis)
  // This is more complicated, as the data are non-contiguous in y.
  for (size_t x = size_x; x--;) {
    // Copy data into column array
    ptr = data + size_xy - size_x + x;
    ptr2 = data_copy + size_y;
    while (ptr2-- > data_copy) {
      *ptr2 = *ptr;
      ptr -= size_x;
    }

    // Apply filter
    // for (size_t i = n_iter; i--;)
    filter_boxcar_1d_flt(data_copy, data_col, size_y, filter_radius);

    // Copy column array back into data array
    ptr = data + size_xy - size_x + x;
    ptr2 = data_copy + size_y;
    while (ptr2-- > data_copy) {
      *ptr = *ptr2;
      ptr -= size_x;
    }
  }
}


void filter_boxcar_1d_flt_riscv(float *data, const size_t size,
                                const size_t stride, const size_t filter_radius,
                                const size_t vl) {
  const size_t filter_size = 2 * filter_radius + 1;
  size_t i;

  const float inv_filter_size = 1 / filter_size;
  float inv_filter_size_arr[vl];
  float zero_arr[vl];
  for (i = 0; i < vl; i++) {
    inv_filter_size_arr[i] = inv_filter_size;
    zero_arr[i] = 0;
  }

  vfloat32m1_t inv_filter_size_v = vle32_v_f32m1(inv_filter_size_arr, vl);
  vfloat32m1_t zero_v = vle32_v_f32m1(zero_arr, vl);

  // TODO: find _mm_malloc equivalent for RISCV
//  vfloat32m1_t *data_copy = (vfloat32m1_t *)malloc(
  //    vl*sizeof(vfloat32m1_t) * (size + 2 * filter_radius));


  vfloat32m1_t *data_copy = (vfloat32m1_t *)malloc(
      vl*32 * (size + 2 * filter_radius));


  for (i = size; i--;) {
    // filtering NaN Values
    // Not using Vector approach since RISCV-V does not have blend instructions
    float data_filtered[vl];
    for (size_t j; j < vl; j++) {
      data_filtered[j] = FILTER_NAN(*(data + (stride * i) + j));
    }
	
	auto index = filter_radius + i;
    data_copy[index] = vle32_v_f32m1(data_filtered, vl);
  }

  for (i = filter_radius; i--;)
    data_copy[i] = data_copy[size + filter_radius + i] = zero_v;

  // Calculate last point
  vfloat32m1_t last_pt_v =  zero_v;
  for (i = filter_size; i--; ) {
    last_pt_v =  vfadd_vv_f32m1 (last_pt_v, data_copy[size + i - 1], vl);
  }
  last_pt_v = vfmul_vv_f32m1(last_pt_v, inv_filter_size_v, vl);

  vse32_v_f32m1(data + (stride * (size - 1)), last_pt_v, vl);  

  vfloat32m1_t next_pt_v = last_pt_v;

  for (int col = size - 1; col--;) {
    vfloat32m1_t current_pt_v =  vfsub_vv_f32m1(data_copy[col], data_copy[filter_size + col], vl);
    
    current_pt_v = vfmul_vv_f32m1(current_pt_v, inv_filter_size_v, vl);
    
    current_pt_v = vfadd_vv_f32m1(next_pt_v, current_pt_v, vl);

    vse32_v_f32m1(data + (stride * col), current_pt_v, vl);

    next_pt_v = current_pt_v;
  }

  free(data_copy);

}

void filter_gauss_2d_flt_riscv(float *data, float *data_copy, float *data_row,
                               float *data_col, size_t size_x, size_t size_y,
                               size_t n_iter, size_t filter_radius) {
  // Set up a few variables
  const size_t size_xy = size_x * size_y;
  float *ptr = data + size_xy;

  // Run row filter (along x-axis)
  // This is straightforward, as the data are contiguous in x.
  while (ptr > data) {
    ptr -= size_x;
    // for (size_t i = n_iter; i--;)
    filter_boxcar_1d_flt(ptr, data_row, size_x, filter_radius);
  }

  // TODO: RISCV-V code for y-axis

  float *ptr2 = data;
  int N = size_x;

  for (size_t vl; size_x > 0; ptr2 += vl, N -= vl) {
    vl = vsetvl_e32m1(N);

    // Apply filter
    // for (size_t i = n_iter; i--;) {
    filter_boxcar_1d_flt_riscv(ptr2, size_y, size_x, filter_radius, vl);
    // }
  }
}

#endif
