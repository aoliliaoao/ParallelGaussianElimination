#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <omp.h>
#include <chrono>

#define NUM_THREADS 9

using namespace std;

// 打印矩阵
void print_matrix(const float *matrix, int dim) {
  std::cout << std::setfill(' '); 
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      std::cout << std::setw(10) << matrix[i * dim + j]; 
    }
    std::cout << '\n';
  }
}

int main(int argc, char* argv[]) {
  cout<<"thread_num="<<NUM_THREADS<<endl;

  int dims[] = {8};

  for (int dim : dims) {
    // 生成随机矩阵
  cout << "dim=" << dim << endl;
  float* m = new float[dim * dim];
  srand(123);
  for (int i = 0; i < dim * dim; i++)
    m[i] = rand() / 100.0f;

  omp_set_num_threads(NUM_THREADS);

  // 计时变量
  std::chrono::duration<double> total_time(0);
  const int num_runs = 20; // 运行次数

  for (int run = 0; run < num_runs; ++run) {
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
      for (int i = 0; i < dim; i++) {
        #pragma omp single
        {
          // 取主元并归一化
          float pivot = m[i * dim + i];
          for (int col = i; col < dim; col++) {
            m[i * dim + col] /= pivot;
          }
        }

        // 并行消元
        #pragma omp for schedule(dynamic)
        for (int elim_row = i + 1; elim_row < dim; elim_row++) {
          float scale = m[elim_row * dim + i];
          for (int col = i; col < dim; col++) {
            m[elim_row * dim + col] -= m[i * dim + col] * scale;
          }
        }
      }
    }

    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    total_time += end_time - start_time;
  }

  // 计算平均时间
  double average_time = total_time.count() / num_runs;
  cout << "Average execution time over " << num_runs << " runs: " << average_time << " seconds" << endl;

  print_matrix(m,dims[0]);
  // 释放内存
  delete[] m;


  }

  return 0;
}