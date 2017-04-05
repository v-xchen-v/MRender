#include <GLFW/glfw3.h>
#include "math.h"

#include "iostream"
using namespace std;

const float pointSize = 0.5;
const float base = 10000;
void drawPoint(float x,float y,float R,float G,float B);
void drawLint_Print(float x1,float y1,float x2,float y2,float R,float G,float B);
void drawLint_DDA(float x1,float y1,float x2,float y2,float R,float G,float B);
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

        // for(float i=0;i<10;i+=0.01)
        // {
        //     drawPoint(i,i,1.0, 0.0, 0.0);
        // }
        drawLint_Print(0,0,1,1,1.0,0.0,0.0);
        drawLint_DDA(0,0,1,0.5,1.0,0.0,0.0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/*
x,y - position R,G,B - color
*/
void drawPoint(float x,float y,float R,float G,float B,float pointSize)
{
    /* Draw a point */      
    glPointSize(pointSize);
    glBegin(GL_POINTS);
        glColor3f(R,G,B);
        glVertex2f(x,y);
    glEnd();
}
/*
Line
逐点比较法：直线生成算法中最简单的一种
算法：
1）根据直线的斜率(|k|>=1:y |k|<1:x)确定是以X还是以Y方向前进一步
2）前进一个像素点，并更新斜率
重复1）2）
*/
void drawLint_Print(float x1,float y1,float x2,float y2,float R,float G,float B)
{
    double dx,dy,temp_x = x1,temp_y = y1;
    dx = x2-x1;
    dy = y2-y1;
    double k = fabs(dy/dx);
    while(1)
    {
        if(k>=1)
        {
            if(dy>0)
            {
                temp_y+=1/base;
                if(temp_y>y2)
                    break;
            }
            else
            {
                temp_y-=1/base;
                if(temp_y<y2)
                    break;
            }
            drawPoint(temp_x,temp_y,R,G,B,pointSize);
            k = fabs((y2-temp_y)/(x2-temp_x));
        }else
        {
            if(dx>0)
            {
                temp_x+=1/base;
                if(temp_x > x2)
                    break;
            }
            else
            {
                temp_x-=1/base;
                if(temp_x <x2)
                    break;
            }
            drawPoint(temp_x,temp_y,R,G,B,pointSize);
            k = fabs((y2-temp_y)/(x2-temp_x));
        }
    }
}
/*
Line
DDA(数值微分划线算法)
算法：
1）根据直线的斜率(|k|>=1:y |k|<1:x)确定是以X还是以Y方向前进1个距离,另一个方向前进1/k个距离
2）前进后重新进入步骤1）
重复1）2）
*/
void drawLint_DDA(float x1,float y1,float x2,float y2,float R,float G,float B)
{
    float dm = 0,dx = 0,dy = 0;
    if(fabs(x2-x1) >= fabs(y2-y1))
    {
        dm = fabs(x2-x1);
    }else
    {
        dm = fabs(y2-y1);
    }
    dx = (float)(x2-x1)/(dm*base);
    dy = (float)(y2-y1)/(dm*base);
    for(float i=0;i<dm*base;i++)
    {;
        drawPoint(x1,y1,R,G,B,pointSize);
        x1+=dx;
        y1+=dy;
    }
}

