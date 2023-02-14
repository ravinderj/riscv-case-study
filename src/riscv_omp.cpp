// WIP

// #include "omp.h"
#include <cstdio>
#include <iostream>
#include <vector>

int add(int a, int b) { return a + b; }

int main() {
  std::vector<int> a = {1, 2, 3, 4, 5, 6};
  std::vector<int> b = {1, 2, 3, 4, 5, 6};
// #pragma omp parallel for
  for (int i = 0; i < a.size(); i++) {
    printf("sum of %d and %d =  %d\n", a[i], b[i], add(a[i], b[i]));
    // printf("sum of %d and %d =  %d\n", 5, 6, add(5, 6));
  }
}