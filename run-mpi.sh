#!/bin/bash  
  
# 定义要运行的进程数数组  
np_array=(1 2 3 4 5 6 7 8)  
  
# 遍历数组中的每个进程数  
for np in "${np_array[@]}"  
do  
  echo "Running with $np processes..."  
  # 使用mpirun运行MPI程序，指定进程数  
  mpirun -n $np ./mpi  
  # 可选：如果需要在每次运行后等待用户输入再继续，可以取消以下行的注释  
  # read -p "Press Enter to continue with the next number of processes..."  
done  
  