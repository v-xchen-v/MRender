#include "iostream"
#include "vector"

#include "matrix.h"
using namespace std;
vector<vector<float>> matrix_multiply(vector<vector<float>> arrA,vector<vector<float>> arrB)
{
    //矩阵A的行数
    int rowA = arrA.size();
    //矩阵B的行数
    int rowB = arrB.size();
    //矩阵A的列数
    int colA = arrA[0].size();
    //矩阵B的列数
    int colB = arrB[0].size();
    //相乘后的矩阵
    vector<vector<float>> result;
    if(colA != rowB)//如果矩阵A的列数不等于矩阵B的行数
    {
        return result;//返回空
    }else
    {
        //设置矩阵行数，初始时为0
        result.resize(rowA);
        for(int i=0;i<rowA;i++)
        {
            result[i].resize(colB);
        }

        //矩阵相乘
        for(int i=0;i<rowA;i++)
        {
            for(int j=0;j<colB;j++)
            {
                for(int k=0;k<colA;k++)
                {
                    result[i][j] += arrA[i][k]*arrB[k][j];
//                    cout<<result[i][j]<<"+="<<arrA[i][k]<<"*"<<
                }
            }
        }
    }
    return result;
}