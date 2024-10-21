#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <chrono>
#include <vector>

using namespace std;

void print_matrix(const float **matrix,int dim){
    cout<<setfill(' ');
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            cout<<setw(10)<<matrix[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
}

void gaussian_elimination(float **matrix,int dim){
    for(int row=0;row<dim;row++){
        //取主元
        float pivot=matrix[row][row];

        //归一化
        for(int col=0;col<dim;col++){
            matrix[row][col]/=pivot;
        }

        //消元
        for(int elim_row=row+1;elim_row<dim;elim_row++){
            float scale=matrix[elim_row][row];
            for(int col=0;col<dim;col++){
                matrix[elim_row][col]-=scale*matrix[row][col];
            }
        }
    }
}

int main(){
    //测试的维度列表
    int dims[]={8};
    int len_dims=sizeof(dims)/sizeof(dims[0]);

    //运行次数
    int num_runs=1;

    for(int dim=0;dim<len_dims;dim++){
        cout<<"Testing dimension:"<<dim<<endl;

        //生成随机矩阵
        float** matrix=new float[dim][dim];
        

    }
    return 0;
}