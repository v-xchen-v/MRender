
#ifndef MATRIX_H
#define MATRIX_H
// class point2D;

#include <vector>
#include <iostream>
#include <math.h>

#include "draw2D.h"
using namespace std;
const float PI = 3.1415926;
class matrixPoint
{
public:
	vector<vector<float>>matrix;
	matrixPoint(){
		init();
	};
	matrixPoint(point2D point)
	{
		init();
		matrix[0][0] = point.x;
		matrix[0][1] = point.y;
		matrix[0][2] = 1;
	};
	void setValue(point2D point)
	{
		matrix[0][0] = point.x;
		matrix[0][1] = point.y;
		matrix[0][2] = 1;
	}
	void init()
	{
		matrix.resize(1);
		matrix[0].resize(3);
	};
	vector<vector<float>>getMatrixData()
	{
		return matrix;
	};
};

/*
3*3矩阵
*/
class matrix2D
{
public:
	vector<vector<float>>matrix;
	matrix2D()
	{
		reset();
	};
	void reset()
	{
		matrix.resize(3);
		for(int i=0;i<3;i++)
		{
			matrix[i].resize(3);
		}
		matrix[0][0] = 1;
		matrix[1][1] = 1;
		matrix[2][2] = 1;
	}
	//单位化矩阵
	void identity()
	{
		for(int i=0;i<3;i++)
		{
			matrix[i][i] = 1;
		}
	}
	//缩放，x方向缩放比例为Sx，y方向的缩放比例为Sy
	void resizer(float Sx,float Sy)
	{
		matrix[0][0] = Sx;
		matrix[1][1] = Sy;
		matrix[2][2] = 1;
	}
	/*
	以逆时针为正角度
	旋转a度

	x' = xcosa - ysina;
	y' = xsina + ycosa; 
	[x' y' 1] = [x y 1][ cosa sina 0]
	                   [-sina cosa 0]
	                   [   0   0   1]

	弧度与角度的关系为：
	弧度 = 角度*π / 180 
	C++ 中的三角函数都是使用弧度值
	*/
	void rotate(float a)
	{
		matrix[0][0] = cos(a*PI/180);
		matrix[0][1] = sin(a*PI/180);
		matrix[1][0] = -sin(a*PI/180);
		matrix[1][1] = cos(a*PI/180);
		matrix[2][2] = 1;
	}
	vector<vector<float>> getMatrixData()
	{
		return matrix;
	};
};

//矩阵乘法
vector<vector<float>> matrix_multiply(vector<vector<float>> arrA,vector<vector<float>> arrB);
#endif