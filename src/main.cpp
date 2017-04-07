#include "draw2D.h"
#include "matrix.h"

#include <GLFW/glfw3.h>
#include "math.h"

#include "iostream"
#include <vector>

using namespace std;
const float R = 1.0;
const float G = 0.0;
const float B = 0.0;
/*
简单粗暴的画线，画立方体
*/
/*
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
        drawLine_DDA(vertexList[lineVIndex[i][0]][0],vertexList[lineVIndex[i][0]][1],vertexList[lineVIndex[i][1]][0],vertexList[lineVIndex[i][1]][1],R,G,B);
    }
}
*/
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
/*
vector<vector<float>>IOProjection(vector<vector<float>>vc)
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
*/
void testDrawPoint()
{
    for(float i=0;i<10;i+=0.01)
    {
        point2D point(i,i);
        drawPoint(point,1.0, 0.0, 0.0, mrender_pointSize);
    }    
}
void testDrawLine()
{
    point2D point1(0,0);
    point2D point2(1,0.5);
    drawLine_Print(point1,point2,1.0,0.0,0.0);
    // drawLine_DDA(point1,point2,1.0,0,0);
}
void testDrawTriangle()
{
    point2D point3(-0.25,0.6);
    point2D point2(0.25,0.6);
    point2D point1(0,1);
    drawTriangle_flatBottom(point1,point2,point3,1.0,0.0,0.0);

    point2D point4(0,0);
    point2D point5(-0.25,0.4);
    point2D point6(0.25,0.4);
    drawTriangle_flatTop(point5,point6,point4,0.0,1.0,0.0);

    point2D point7(0,0);
    point2D point8(-1,-0.75);
    point2D point9(-0.5,-0.25);
    drawTriangle(point7,point8,point9,0.0,0.0,1.0);
}
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Show Me Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // testDrawPoint();
        // testDrawLine();
        testDrawTriangle();
        
        // vector<vector<float>>cubeVertex = 
        // {
        //     {-0.5f, -0.5f, -0.5f}, 
        //     {0.5f, -0.5f, -0.5f}, 
        //     {-0.5f, 0.5f, -0.5f}, 
        //     {0.5f, 0.5f, -0.5f}, 
        //     {-0.5f, -0.5f, 0.5f}, 
        //     {0.5f, -0.5f, 0.5f}, 
        //     {-0.5f, 0.5f, 0.5f}, 
        //     {0.5f, 0.5f, 0.5f} 
        // };
        // vector<vector<float>>after = IOProjection(cubeVertex);
        // for(int i=0;i<after.size();i++)
        // {
        //     cout<<"line "<<i<<":";
        //     for(int j=0;j<after[0].size();j++)
        //     {
        //         cout<<after[i][j]<<" ";
        //     }
        //     cout<<endl;
        // }
        // drawCube(after);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}