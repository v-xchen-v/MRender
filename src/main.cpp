#include <GLFW/glfw3.h>
#include "math.h"

#include "iostream"
#include "vector"
using namespace std;

const float mrender_pointSize = 0.5;
const float base = 10000;
void drawPoint(float x,float y,float R,float G,float B);
void drawLine_Print(float x1,float y1,float x2,float y2,float R,float G,float B);
void drawLine_DDA(float x1,float y1,float x2,float y2,float R,float G,float B);
void drawTriangle_flatTop(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B);
void drawTriangle_flatBottom(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B);
void drawTriangle(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B);
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
        // drawLint_Print(0,0,1,1,1.0,0.0,0.0);
        drawLine_DDA(1,1,0,0,1.0,0.0,0.0);
        drawTriangle(0,0.5,-1,-0.25,-0.5,0.0,1.0,0.0,0.0);
        // drawTriangle(-0.5,-0.5,0.5,-0.5,0.0,0.0,1.0,0.0,0.0);
        // drawTriangle(-0.25,0.5,0.25,0.5,0,0,1.0,0.0,0.0);
        // drawTriangle_flatTop(0,0,-0.5,-0.5,0.5,-0.5,1.0,0.0,0.0);
        // drawTriangle_flatBottom(-0.25,0.5,0.25,0.5,0,0,1.0,0.0,0.0);

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
    glPointSize(mrender_pointSize);
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
void drawLine_Print(float x1,float y1,float x2,float y2,float R,float G,float B)
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
            drawPoint(temp_x,temp_y,R,G,B,mrender_pointSize);
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
            drawPoint(temp_x,temp_y,R,G,B,mrender_pointSize);
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
void drawLine_DDA(float x1,float y1,float x2,float y2,float R,float G,float B)
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
        drawPoint(x1,y1,R,G,B,mrender_pointSize);
        x1+=dx;
        y1+=dy;
    }
}

/*
Line
Bresenham画线算法：应用最广泛的直线生成算法
算法；
1）判断k，如果|k|<=1,x每次增1（减1），判断y向是否挪动一个距离
    a）当前位置为(x0,y0),x每次增1，对于直线y=kx+b与x=x0+1的交汇点为当前位置为(x0+1,k(x0+1)+b),设交汇点与(x0+1,y0)距离S,交汇点与(x0+1,y0+1)的距离是T
    b）比较S与T的大小，交汇点距离哪个点近，下一个位置就移动到谁那里。
2）如果|k|>1,y每次增1（减1），判断x是否挪动1个距离
    类似
重复1）2）
*/
// void drawLine_Bres(float x1,float y1,float x2,float y2,float R,float G,float B)
// {
    // float dx = 0,k = 0,dy = 0,S,T,tmp;
    // k = (y2-y1)/(x2-x1);
    // if(abs(k)<=1)
    // {
    //     if(k>0 || k==0&&y2-y1>0)
    //         dx = 1;
    //     else
    //         dx = -1;
    //     drawPoint(x1,y1,R,G,B,mrender_pointSize);
    //     x1 += dx;
    //     tmp = k(x1+1)+
    // }
// }

/*
         (x1,y2)
        /      \
       /        \
      /          \
  (x2,y2)——————-(x3,y3)
  y不断增加，增至yi的时候，Xl = (yi-y1)*(x2-x1)/(y2-y1);
                        Xr = (yi-y1)*(x3-x1)/(y3-y1);
    

*/
void drawTriangle_flatBottom(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B)
{
    for(float i = y1; i>=y2 ;i-=1/base)
    {
        float xl = (i-y1)*(x2-x1)/(y2-y1)+x1;
        float xr = (i-y1)*(x3-x1)/(y3-y1)+x1;
        cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatBottom"<<endl;
        drawLine_DDA(xl,i,xr,i,R,G,B);
    }
}
/*
        (x1,y1)--------(x2,y2)
            \             /
             \           /
              \         /
               \       /
                (x3,y3)
*/
void drawTriangle_flatTop(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B)
{
    for(float i = y1; i>=y3 ;i-=1/base)
    {
        float xl = (i-y3)*(x1-x3)/(y1-y3)+x3;
        float xr = (i-y3)*(x2-x3)/(y2-y3)+x3;
        cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatTop"<<endl;
        drawLine_DDA(xl,i,xr,i,R,G,B);
    }
}
/*
                (xtop,ytop)
                      /|
                     / |
                    /  |
                   /   |     
                  /____|
      (xmid,ymid) \    |(xtmp,ymid)
                   \   |
                    \  |
                     \ |
                      \| 
                      (xbottom,ybottom)
*/
bool comp(pair<float,float>y_pair1,pair<float,float>y_pair2)
{
    return y_pair1.second>y_pair2.second; //float浮点数比较？
}
void drawTriangle(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B)
{
    float xtop,ytop,xmid,ymid,xtmp,xbottom,ybottom;
    if(y1 == y2)
    {
        if(y3>=y1)
        {
            drawTriangle_flatBottom(x3,y3,x1,y1,x2,y2,R,G,B);
        }else
        {
            drawTriangle_flatTop(x1,y1,x2,y2,x3,y3,R,G,B);
        }
    }else if(y1 == y3)
    {
        if(y2>= y1)
        {
            drawTriangle_flatBottom(x2,y2,x1,y1,x3,y3,R,G,B);
        }else
        {
            drawTriangle_flatTop(x1,y1,x3,y3,x2,y2,R,G,B);
        }
    }else if(y2 == y3)
    {
        if(y1>=y2)
        {
            drawTriangle_flatBottom(x1,y1,x2,y2,x3,y3,R,G,B);
        }else
        {
            drawTriangle_flatTop(x2,y2,x3,y3,x1,y1,R,G,B);
        }
    }else //如果不是平顶、平底三角形
    {
        vector<pair<float,float>>vc;
        vc.push_back(make_pair(x1,y1));
        vc.push_back(make_pair(x2,y2));
        vc.push_back(make_pair(x3,y3));
        sort(vc.begin(),vc.end(),comp);
        xtop = vc[0].first;
        ytop = vc[0].second;
        xmid = vc[1].first;
        ymid = vc[1].second;
        xbottom = vc[2].first;
        ybottom = vc[2].second;
        xtmp = (ymid-ytop)*(xbottom-xtop)/(ybottom-ytop)+xtop;
        cout<<xtop<<" "<<ytop<<" "<<xmid<<" "<<ymid<<" "<<xbottom<<" "<<ybottom<<" "<<xtmp<<endl;
        drawTriangle_flatBottom(xtop,ytop,xtmp,ymid,xmid,ymid,R,G,B);
        drawTriangle_flatTop(xmid,ymid,xtmp,ymid,xbottom,ybottom,R,G,B);
    }
}