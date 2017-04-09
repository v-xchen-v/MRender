#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#include "draw3D.h"
#include "draw2D.h"
/*
简单粗暴的画线，画立方体
*/

const float R = 1.0;
const float G = 0.0;
const float B = 0.0;
void drawCube(vector<vector<float>>vertexList)
{
    //一个12*2的二维数组，用于存放立方体线的两端顶点号
    vector<vector<int>>lineVIndex = {
        {0, 1},    
        {2, 3},    
        {4, 5},    
        {6, 7},    
        {0, 2},    
        {1, 3},    
        {4, 6},    
        {5, 7},
        {0, 4},
        {1, 5},
        {7, 3},
        {2, 6}
    };
    for(int i=0;i<12;i++)
    {
        point2D point1(vertexList[lineVIndex[i][0]][0],vertexList[lineVIndex[i][0]][1]);
        point2D point2(vertexList[lineVIndex[i][1]][0],vertexList[lineVIndex[i][1]][1]);
        drawLine_DDA(point1,point2,R,G,B);
    }
}

// isometric orthographic projection  正等侧面投影
/*
将3D实体绕z轴逆时针旋转a角
将3D实体绕x轴顺时针旋转b角
向xoz平面作正投影
变换矩阵为T =
            【cosa        -sina     0    0】
            【     0        0       0    0】
            【-sinasinb -cosasinb  cosb  0】
            【     0        0       0    1】
a = 35度 b = 35度16’
A(x,y,z)->T*A->= A'(cosax-sinay,0,-sinasinbx-cosasinby+cosbz)
*/

vector<vector<float>> IOProjection(vector<vector<float>>vc)
{
    vector<vector<float>>afterP(8,vector<float>(2));
    float a = 35.0*3.14/180.0;
    float b = 35.266666*3.14/180.0;
    for(int i=0;i<vc.size();i++)
    {
        float x = vc[i][0];
        float y = vc[i][1];
        float z = vc[i][2];
        float newX = cos(a)*x-sin(a)*y;
        //float newY = 0;
        float newZ = -sin(a)*sin(b)*x + -cos(a)*sin(b)*y + cos(b)*z;
        afterP[i][0] = newX;
        afterP[i][1] = newZ;
    }
return afterP;
}
