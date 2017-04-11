#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#include "draw3D.h"
#include "draw2D.h"
#include "settings.h"
using namespace std;

/*
简单粗暴的画线，画立方体
*/

const float R = 1.0;
const float G = 0.0;
const float B = 0.0;
void drawCube_wireframe(vector<vector<float>>vertexList)
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
        cout<<"line:"<<i<<" "<<vertexList[lineVIndex[i][0]][0]<<" "<<vertexList[lineVIndex[i][0]][1]<<" | "<<vertexList[lineVIndex[i][1]][0]<<" "<<vertexList[lineVIndex[i][1]][1]<<endl;
        point2D point1(vertexList[lineVIndex[i][0]][0],vertexList[lineVIndex[i][0]][1]);
        point2D point2(vertexList[lineVIndex[i][1]][0],vertexList[lineVIndex[i][1]][1]);
        drawLine_DDA(point1,point2,R,G,B);
    }
}

/*
立方体有8个顶点6个面，每个面有4个顶点，每个面都可以分成两个三角形画出
画一个立方体，就是画12个三角形
*/
void drawCube_fillcolor(vector<vector<float>>vertexList)
{
    //一个12*2的二维数组，用于存放立方体线的两端顶点号
    vector<vector<int>>lineVIndex = {
        // {0, 1},    
        // {2, 3},    
        // {4, 5},    
        // {6, 7},    
        // {0, 2},    
        // {1, 3},    
        // {4, 6},    
        // {5, 7},
        // {0, 4},
        // {1, 5},
        // {7, 3},
        // {2, 6}
        //通过固定画面的顺序，强行消隐藏:从远到近画物体（多边形），近的就因为优先级高而覆盖远的多边形。由此可消隐。
        {0, 1 ,3},    
        {0, 2, 3},  
        {0, 4, 6},    
        {0, 2, 6},
        {1, 4, 5},
        {0, 1, 4},    
        {6, 3, 7},    
        {6, 2, 3}, 
        {4, 5, 7},    
        {4, 6, 7},
        {1, 5, 7},
        {1, 3, 7}
    };
    for(int i=0;i<12;i++)
    {
        point2D point1(vertexList[lineVIndex[i][0]][0],vertexList[lineVIndex[i][0]][1]);
        point2D point2(vertexList[lineVIndex[i][1]][0],vertexList[lineVIndex[i][1]][1]);
        point2D point3(vertexList[lineVIndex[i][2]][0],vertexList[lineVIndex[i][2]][1]);
        if(i%2==0)
            drawTriangle(point1,point2,point3,1,0,0);
        else
            drawTriangle(point1,point2,point3,0,0,1);
    }
}

void setCubeColor_fillcolor(vector<vector<float>>vertexList)
{
    //一个12*2的二维数组，用于存放立方体线的两端顶点号
    vector<vector<int>>lineVIndex = {
        // {0, 1},    
        // {2, 3},    
        // {4, 5},    
        // {6, 7},    
        // {0, 2},    
        // {1, 3},    
        // {4, 6},    
        // {5, 7},
        // {0, 4},
        // {1, 5},
        // {7, 3},
        // {2, 6}
        //通过固定画面的顺序，强行乱序遮盖:使用zBuffer深度浅的更新像素点
        {1, 5, 7},
        {1, 3, 7},
        {0, 1 ,3},    
        {0, 2, 3},  
        {0, 4, 6},    
        {0, 2, 6},
        {1, 4, 5},
        {0, 1, 4},    
        {6, 3, 7},    
        {6, 2, 3}, 
        {4, 5, 7},    
        {4, 6, 7}  
    };
    for(int i=0;i<12;i++)
    {
        point2D point1(vertexList[lineVIndex[i][0]][0],vertexList[lineVIndex[i][0]][1]);
        point2D point2(vertexList[lineVIndex[i][1]][0],vertexList[lineVIndex[i][1]][1]);
        point2D point3(vertexList[lineVIndex[i][2]][0],vertexList[lineVIndex[i][2]][1]);
        float depth1 = vertexList[lineVIndex[i][0]][2];
        float depth2 = vertexList[lineVIndex[i][1]][2];
        float depth3 = vertexList[lineVIndex[i][2]][2];
        float depth = min(min(depth1,depth2),depth3);
        cout<<i<<"th Triangle depth:"<<depth<<endl;
        {
            if(i%2==0)
            {
                setTriangleColor(point1,point2,point3,1,0,0,depth);
            }
            else
            {
                setTriangleColor(point1,point2,point3,0,0,1,depth);
            }
        }
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
    vector<vector<float>>afterP(8,vector<float>(3));
    float a = 35.0*3.14/180.0;
    float b = 35.266666*3.14/180.0;
    for(int i=0;i<vc.size();i++)
    {
        float x = vc[i][0];
        float y = vc[i][1];
        float z = vc[i][2];
        float newX = cos(a)*x-sin(a)*y;
        float newY = y;
        float newZ = -sin(a)*sin(b)*x + -cos(a)*sin(b)*y + cos(b)*z;
        afterP[i][0] = newX;
        afterP[i][1] = newZ;
        afterP[i][2] = newY;//深度
    }
return afterP;
}
//绕x轴旋转a度
/*
A(x,y,x)->A'(x,cosay-sinaz,sinay+cosaz)
*/
vector<vector<float>> RotatebyX(vector<vector<float>>vc,float a)
{
    a = a*3.14/180.0;
    vector<vector<float>>afterP(8,vector<float>(3));
    for(int i=0;i<vc.size();i++)
    {
        float x = vc[i][0];
        float y = vc[i][1];
        float z = vc[i][2];
        float newX = x;
        float newY = cos(a)*y-sin(a)*z;
        float newZ = sin(a)*y+cos(a)*z;
        afterP[i][0] = newX;
        afterP[i][1] = newY;
        afterP[i][2] = newZ;
    }
return afterP;
}

//绕z轴旋转a度
/*
A(x,y,z)->A'(x',y',z')

x' = xcosa - ysina;
y' = xsina + ycosa; 
[x' y' z'] = [x y z][ cosa sina 0]
                   [-sina cosa 0]
                   [   0   0   1]
*/
vector<vector<float>> RotatebyZ(vector<vector<float>>vc,float a)
{
    a = a*3.14/180.0;
    vector<vector<float>>afterP(8,vector<float>(3));
    for(int i=0;i<vc.size();i++)
    {
        float x = vc[i][0];
        float y = vc[i][1];
        float z = vc[i][2];
        float newX = cos(a)*x-sin(a)*y;
        float newY = sin(a)*x+cos(a)*y;
        float newZ = z;
        afterP[i][0] = newX;
        afterP[i][1] = newY;
        afterP[i][2] = newZ;
        cout<<"before RotatebyZ "<<x<<" "<<y<<" after "<<newX<<" "<<newY<<endl;
    }
    cout<<"----------"<<endl;
return afterP;
}

//绕y轴旋转a度
/*
A(x,y,z)->A'(x',y',z')

x' = xcosa - zsina;
z' = xsina + zcosa; 
[x' y' z'] = [x y z][ cosa 0  sina ]
                    [-sina 0  cosa ]
                    [   0  1  0    ]
*/
vector<vector<float>> RotatebyY(vector<vector<float>>vc,float a)
{
    a = a*3.14/180.0;
    vector<vector<float>>afterP(8,vector<float>(3));
    for(int i=0;i<vc.size();i++)
    {
        float x = vc[i][0];
        float y = vc[i][1];
        float z = vc[i][2];
        float newX = cos(a)*x+sin(a)*z;
        float newY = y;
        float newZ = -sin(a)*x+cos(a)*z;
        afterP[i][0] = newX;
        afterP[i][1] = newY;
        afterP[i][2] = newZ;
        cout<<"before RotatebyZ "<<x<<" "<<y<<" after "<<newX<<" "<<newY<<endl;
    }
    cout<<"----------"<<endl;
return afterP;
}
/*
将3D图形旋转任意角度，x轴a度，y轴b度，z轴c度
*/
vector<vector<float>> Rotate3D(vector<vector<float>>vc,float a,float b,float c)
{
    vector<vector<float>>afterRotateX(8,vector<float>(3));
    vector<vector<float>>afterRotateXY(8,vector<float>(3));
    vector<vector<float>>afterRotateXYZ(8,vector<float>(3));
    afterRotateX = RotatebyX(vc,a);
    afterRotateXY = RotatebyY(afterRotateX,b);
    afterRotateXYZ = RotatebyZ(afterRotateXY,c);
    return afterRotateXYZ;
}

