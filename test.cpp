#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <omp.h>

#define NUM_THREADS 4

using namespace std;

//打印
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
  //生成随机矩阵
  const int dim = 128;
  cout<<"dim="<<dim<<endl;
  float* m=new float[dim*dim];
  srand(123);
  for (int i = 0; i < dim*dim; i++)
    m[i] = rand() / 100;

  omp_set_num_threads(NUM_THREADS);
  
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
    #pragma omp for
    for (int elim_row = i + 1; elim_row < dim; elim_row++) {
      float scale = m[elim_row * dim + i];
      for (int col = i; col < dim; col++) {
        m[elim_row * dim + col] -= m[i * dim + col] * scale;
      }
    }
  }
}
  
  //print_matrix(m_in,dim);
  return 0;
}

