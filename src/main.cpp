#include "draw2D.h"
#include "matrix.h"
#include "draw3D.h"
#include "settings.h"

#include <GLFW/glfw3.h>
#include "math.h"

#include "iostream"
#include <vector>

using namespace std;
const float R = 1.0;
const float G = 0.0;
const float B = 0.0;
vector<vector<zBufferPoint>> zBufferMatrix(1000,vector<zBufferPoint>(1000));
void showZbufferGraphcis()
{
    //draw by zBufferMatrix
        // int count =0;
        for(int i=0;i<base*2;i++)
        {
            for(int j=0;j<base*2;j++)
            {
                if(zBufferMatrix[i][j].R)
                {
                    // count++;
                    // cout<<"R:1 "<<count<<" "<<i<<" "<<j<<endl;
                }
                // cout<<"i-j"<<i<<"-"<<j<<" R "<<zBufferMatrix[i][j].R<<" G "<<zBufferMatrix[i][j].G<<" B "<<zBufferMatrix[i][j].B<<endl;
                point2D point((i-base)/base,(j-base)/base);
                drawPoint(point,zBufferMatrix[i][j].R,zBufferMatrix[i][j].G,zBufferMatrix[i][j].B,mrender_pointSize);
            }
        }
}

void testDrawPoint()
{
    for(float i=0;i<10;i+=0.01)
    {
        point2D point(i,i);
        drawPoint(point,1.0, 0.0, 0.0, mrender_pointSize);
    }    
}
void testSetPointColor()
{
    for(float i=0;i<1;i+=0.01)
    {
        point2D point(i,i);
        setPointColor(point,1.0, 0.0, 0.0,-1);
    } 
}
void testDrawLine()
{
    point2D point1(0,0);
    point2D point2(1,0.5);
    drawLine_Print(point1,point2,1.0,0.0,0.0);
    // drawLine_DDA(point1,point2,1.0,0,0);
}
void testSetLineColor()
{
    point2D point1(0,0);
    point2D point2(1,0.5);
    setLineColor(point1,point2,1, 0, 0,-1);
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
void testSetTriangleColor()
{
    point2D point3(-0.25,0.6);
    point2D point2(0.25,0.6);
    point2D point1(0,1);
    setTriangleColor_flatBottom(point1,point2,point3,1.0,0.0,0.0,-1);

    point2D point4(0,0);
    point2D point5(-0.25,0.4);
    point2D point6(0.25,0.4);
    setTriangleColor_flatTop(point5,point6,point4,0.0,1.0,0.0,-1);

    point2D point7(0,0);
    point2D point8(-1,-0.75);
    point2D point9(-0.5,-0.25);
    setTriangleColor(point7,point8,point9,0.0,0.0,1.0,-1);
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
void testRotate3DXYZ()
{
    vector<vector<float>>cubeVertex = 
    {
        //x,y,z
        {-0.5f, -0.5f, -0.5f}, 
        {0.5f, -0.5f, -0.5f}, 
        {-0.5f, 0.5f, -0.5f}, 
        {0.5f, 0.5f, -0.5f}, 
        {-0.5f, -0.5f, 0.5f}, 
        {0.5f, -0.5f, 0.5f}, 
        {-0.5f, 0.5f, 0.5f}, 
        {0.5f, 0.5f, 0.5f} 
    };
     
    //test rotatex rotatey rotatez
    vector<vector<float>>after = RotatebyX(cubeVertex,5);
    vector<vector<float>>after2 = RotatebyY(after,5);
    vector<vector<float>>after3 = RotatebyZ(after2,5);
    // drawCube_wireframe(after3);
    drawCube_fillcolor(after3);
  
}
void testRotate3D()
{
    vector<vector<float>>cubeVertex = 
    {
        //x,y,z
        {-0.5f, -0.5f, -0.5f}, 
        {0.5f, -0.5f, -0.5f}, 
        {-0.5f, 0.5f, -0.5f}, 
        {0.5f, 0.5f, -0.5f}, 
        {-0.5f, -0.5f, 0.5f}, 
        {0.5f, -0.5f, 0.5f}, 
        {-0.5f, 0.5f, 0.5f}, 
        {0.5f, 0.5f, 0.5f} 
    };
     
    //test rotatex rotatey rotatez
    vector<vector<float>>after = Rotate3D(cubeVertex,15,15,15);
    
    

    // drawCube_fillcolor(after);

    setCubeColor_fillcolor(after);
    showZbufferGraphcis();

    drawCube_wireframe(after);
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
            //x,y,z
            {-0.5f, -0.5f, -0.5f}, 
            {0.5f, -0.5f, -0.5f}, 
            {-0.5f, 0.5f, -0.5f}, 
            {0.5f, 0.5f, -0.5f}, 
            {-0.5f, -0.5f, 0.5f}, 
            {0.5f, -0.5f, 0.5f}, 
            {-0.5f, 0.5f, 0.5f}, 
            {0.5f, 0.5f, 0.5f} 
        };
        // vector<vector<float>>after = IOProjection(cubeVertex);
        // testRotate3DXYZ();
        testRotate3D();
        // drawCube_wireframe(cubeVertex);
        
        // drawCube_fillcolor(after);
        // setCubeColor_fillcolor(after);
        // testSetTriangleColor();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}