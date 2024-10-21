#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <chrono>
#include <vector>

using namespace std;

void print_matrix(vector<vector<float> >matrix){
    cout<<setfill(' ');
    for(int i=0;i<matrix.size();i++){
        for(int j=0;j<matrix[0].size();j++){
            cout<<setw(10)<<matrix[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
}

void gaussian_elimination(vector<vector<float> >&matrix){
    int dim=matrix.size();
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

    for(int i=0;i<len_dims;i++){
        int dim=dims[i];
        cout<<"Testing dimension:"<<dim<<endl;

        //计时变量
        chrono::duration<double> total_time(0);

        vector<vector<float> > matrix(dim,vector<float>(dim));

        for(int run=0;run<num_runs;run++){
            //生成随机矩阵
            srand(123);//随机种子
            for(int i=0;i<dim;i++){
                for(int j=0;j<dim;j++){
                    matrix[i][j]=rand()/100.0f;
                }
            }
            print_matrix(matrix);

            chrono::high_resolution_clock::time_point start_time=chrono::high_resolution_clock::now();

            gaussian_elimination(matrix);

            chrono::high_resolution_clock::time_point end_time=chrono::high_resolution_clock::now();

            total_time+=end_time-start_time;
        }

        //计算平均时间
        double average_time=total_time.count()/num_runs;
        cout << "Average execution time over " << num_runs << " runs: " << average_time << " seconds" << endl;

        print_matrix(matrix);

    }
    return 0;
}