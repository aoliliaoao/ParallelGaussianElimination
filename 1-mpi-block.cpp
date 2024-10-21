#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <iomanip>
#include "mpi.h"
#include <chrono>

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

int main(int argc, char *argv[]) {
  // MPI初始化
  MPI_Init(&argc, &argv);

  // 获得总进程数
  int num_tasks;
  MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

  //计算任务块大小（矩阵行数可以整除进程数）
  const int dim = 8;
  const int n_rows = dim / num_tasks;

  // 获得进程号
  int task_id;
  MPI_Comm_rank(MPI_COMM_WORLD, &task_id);

  //计算chunk起始下标
  const int start_row = task_id * n_rows;
  const int end_row = start_row + n_rows;

  float *matrix = nullptr;

  //存储当前进程的chunk
  float m_chunk[dim*n_rows];

  //缓冲区
  float pivot_row[dim];

  // 0号进程初始化矩阵
  if (task_id == 0) {
    matrix = new float[dim*dim];
    srand(123);
    for (int i = 0; i < dim*dim; i++)
            matrix[i] = rand() / 100.0f;
    cout << "dim=" << dim << endl;
    // print_matrix(matrix,dim);
  }

  // 存储非阻塞通信句柄
  std::vector<MPI_Request> requests(num_tasks);

  // 计时变量
  std::chrono::duration<double> total_time(0);
  const int num_runs = 10; // 运行次数
  for (int run = 0; run < num_runs; ++run) {
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    
    // 分散chunk
    MPI_Scatter(matrix, dim * n_rows, MPI_FLOAT, m_chunk,
                dim * n_rows, MPI_FLOAT, 0, MPI_COMM_WORLD);

    //高斯消元
    for (int row = 0; row < end_row; row++) {
      int mapped_rank = row / n_rows;

      // 当前行属于当前进程的chunk
      if (task_id == mapped_rank) {
        // chunk中的相对行下标
        int local_row = row % n_rows;

        // 取主元
        float pivot = m_chunk[local_row * dim + row];

        // 矩阵剩余元素除以当前行主元,使主元位置处元素为1
        for (int col = row; col < dim; col++) {
          m_chunk[local_row * dim + col] /= pivot;
        }

        //非阻塞通信
        for (int i = mapped_rank + 1; i < num_tasks; i++) {
          MPI_Isend(m_chunk + dim * local_row, dim, MPI_FLOAT, i, 0,
                    MPI_COMM_WORLD, &requests[i]);
        }

        // 消元
        for (int elim_row = local_row + 1; elim_row < n_rows; elim_row++) {
          float scale = m_chunk[elim_row * dim + row];

          for (int col = row; col < dim; col++) {
            m_chunk[elim_row * dim + col] -=
                m_chunk[local_row * dim + col] * scale;
          }
        }

        for (int i = mapped_rank + 1; i < num_tasks; i++) {
          MPI_Wait(&requests[i], MPI_STATUS_IGNORE);
        }
      } 

      // 当前行不属于当前进程的chunk
      else {
        //接收消息
        MPI_Recv(pivot_row, dim, MPI_FLOAT, mapped_rank, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        //消元
        for (int elim_row = 0; elim_row < n_rows; elim_row++) {
          float scale = m_chunk[elim_row * dim + row];

          for (int col = row; col < dim; col++) {
            m_chunk[elim_row * dim + col] -= pivot_row[col] * scale;
          }
        }
      }
    }
 
    // 收集结果给0号进程
    MPI_Gather(m_chunk, n_rows * dim, MPI_FLOAT, matrix, n_rows * dim,
               MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    total_time += end_time - start_time;
  }
  MPI_Finalize();

  // 计算平均时间
  if (task_id == 0) {
    double average_time = total_time.count() / num_runs;
    cout << "Average execution time over " << num_runs << " runs: " << average_time << " seconds" << endl;
  }
  
  // 只在根进程中释放matrix
  if (task_id == 0 && matrix != nullptr) {
    print_matrix(matrix,dim);
    delete[] matrix;
  }

  return 0;
}