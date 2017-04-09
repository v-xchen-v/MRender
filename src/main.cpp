#include "draw2D.h"
#include "matrix.h"
#include "draw3D.h"

#include <GLFW/glfw3.h>
#include "math.h"

#include "iostream"
#include <vector>

using namespace std;
const float R = 1.0;
const float G = 0.0;
const float B = 0.0;

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
void testResize2D()
{
    point2D point7(0,0);
    point2D point8(-1,-0.75);
    point2D point9(-0.5,-0.25);
    resizer2DGraphics(point7,point8,point9,0.1,0.1);
    // drawTriangle(point7,point8,point9,0.0,0.0,1.0);

}

void testTranslate2D()
{
    point2D point7(0,0);
    point2D point8(-1,-0.75);
    point2D point9(-0.5,-0.25);
    translate2DGraphics(point7,point8,point9,1,1);
}
void testRotate2D()
{
    point2D point7(0,0);
    point2D point8(-1,-0.75);
    point2D point9(-0.5,-0.25);
    rotate2DGraphcis(point7,point8,point9,90);
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

        /* 2D Graphcis test examples*/
        // testDrawPoint();
        // testDrawLine();
        // testDrawTriangle();
        // testResize2D();
        // testTranslate2D();
        // testRotate2D();


        vector<vector<float>>cubeVertex = 
        {
            {-0.5f, -0.5f, -0.5f}, 
            {0.5f, -0.5f, -0.5f}, 
            {-0.5f, 0.5f, -0.5f}, 
            {0.5f, 0.5f, -0.5f}, 
            {-0.5f, -0.5f, 0.5f}, 
            {0.5f, -0.5f, 0.5f}, 
            {-0.5f, 0.5f, 0.5f}, 
            {0.5f, 0.5f, 0.5f} 
        };
        vector<vector<float>>after = IOProjection(cubeVertex);
        // for(int i=0;i<after.size();i++)
        // {
        //     cout<<"line "<<i<<":";
        //     for(int j=0;j<after[0].size();j++)
        //     {
        //         cout<<after[i][j]<<" ";
        //     }
        //     cout<<endl;
        // }
        drawCube(after);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}