#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <chrono>

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
  cout << endl;
}

// 高斯消元
void gaussian_elimination(float *m, int dim) {
  for (int row = 0; row < dim; row++) {
    // 取主元
    float pivot = m[row * dim + row];

    // 矩阵剩余元素除以当前行主元,使主元位置处元素为1
    for (int col = row; col < dim; col++) {
      m[row * dim + col] /= pivot;
    }

    for (int elim_row = row + 1; elim_row < dim; elim_row++) {
      // 获得乘数
      float scale = m[elim_row * dim + row];

      // 消元
      for (int col = row; col < dim; col++) {
        m[elim_row * dim + col] -= m[row * dim + col] * scale;
      }
    }
  }
}

int main() {
  // 测试的维度列表
  int dims[] = {8};
  const int num_runs = 1; // 运行次数

  for (int dim : dims) {
    cout << "Testing dimension: " << dim << endl;

    // 生成随机矩阵
    float* m = new float[dim * dim];
    srand(123); // 随机种子
    for (int i = 0; i < dim * dim; i++)
      m[i] = rand() / 100.0f;

    // 计时变量
    std::chrono::duration<double> total_time(0);

    for (int run = 0; run < num_runs; ++run) {
      std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

      // 高斯消元
      gaussian_elimination(m, dim);

      std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
      total_time += end_time - start_time;
    }

    // 计算平均时间
    double average_time = total_time.count() / num_runs;
    cout << "Average execution time over " << num_runs << " runs: " << average_time << " seconds" << endl;

    print_matrix(m,dim);

    // 释放内存
    delete[] m;
  }
  

  return 0;
}