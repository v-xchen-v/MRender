#include "draw2D.h"
#include <GLFW/glfw3.h>
#include "math.h"
#include "iostream"
#include "vector"

using namespace std;

const float mrender_pointSize = 0.5;
const float base = 1000;

/*
x,y - position R,G,B - color
*/
void drawPoint(point2D point,float R,float G,float B,float pointSize)
{
    /* Draw a point */      
    glPointSize(mrender_pointSize);
    glBegin(GL_POINTS);
        glColor3f(R,G,B);
        glVertex2f(point.x,point.y);
    glEnd();
}
/*
Line
逐点比较法：直线生成算法中最简单的一种
算法：
1）根据直线的斜率(|k|>=1:y |k|<1:x)确定是以X还是以Y方向前进一步
2）前进一个像素点，并更新斜率
重复1）2）
有问题，待修改
*/
void drawLine_Print(point2D point1,point2D point2,float R,float G,float B)
{
    double dx,dy,temp_x = point1.x,temp_y = point1.y;
    dx = point2.x-point1.x;
    dy = point2.y-point1.y;
    double k = fabs(dy/dx);
    while(1)
    {
        if(k>=1)
        {
            if(dy>0)
            {
                temp_y+=1/base;
                if(temp_y>point2.y)
                    break;
            }
            else
            {
                temp_y-=1/base;
                if(temp_y<point2.y)
                    break;
            }
            point2D tempP(temp_x,temp_y);
            drawPoint(tempP,R,G,B,mrender_pointSize);
            k = fabs((point2.y-temp_y)/(point2.x-temp_x));
        }else
        {
            if(dx>0)
            {
                temp_x+=1/base;
                if(temp_x > point2.x)
                    break;
            }
            else
            {
                temp_x-=1/base;
                if(temp_x < point2.x)
                    break;
            }
            point2D tempP(temp_x,temp_y);
            drawPoint(tempP,R,G,B,mrender_pointSize);
            k = fabs((point2.y-temp_y)/(point2.x-temp_x));
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
void drawLine_DDA(point2D point1,point2D point2,float R,float G,float B)
{
    float dm = 0,dx = 0,dy = 0;
    if(fabs(point2.x-point1.x) >= fabs(point2.y-point1.y))
    {
        dm = fabs(point2.x-point1.x);
    }else
    {
        dm = fabs(point2.y-point1.y);
    }
    dx = (float)(point2.x-point1.x)/(dm*base);
    dy = (float)(point2.y-point1.y)/(dm*base);
    for(float i=0;i<dm*base;i++)
    {
        point2D tempPoint(point1.x,point1.y);
        drawPoint(tempPoint,R,G,B,mrender_pointSize);
        point1.x+=dx;
        point1.y+=dy;
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
         (x1,y1)
        /      \
       /        \
      /          \
  (x2,y2)——————-(x3,y3)
  y不断增加，增至yi的时候，Xl = (yi-y1)*(x2-x1)/(y2-y1);
                        Xr = (yi-y1)*(x3-x1)/(y3-y1);
    

*/
                        
void drawTriangle_flatBottom(point2D point1,point2D point2,point2D point3,float R,float G,float B)
{
    for(float i = point1.y; i>=point2.y ;i-=1/base)
    {
        float xl = (i-point1.y)*(point2.x-point1.x)/(point2.y-point1.y)+point1.x;
        float xr = (i-point1.y)*(point3.x-point1.x)/(point3.y-point1.y)+point1.x;
        cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatBottom"<<endl;
        point2D point1(xl,i);
        point2D point2(xr,i);
        drawLine_DDA(point1,point2,R,G,B);
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
void drawTriangle_flatTop(point2D point1,point2D point2,point2D point3,float R,float G,float B)
{
    for(float i = point1.y; i>=point3.y ;i-=1/base)
    {
        float xl = (i-point3.y)*(point1.x-point3.x)/(point1.y-point3.y)+point3.x;
        float xr = (i-point3.y)*(point2.x-point3.x)/(point2.y-point3.y)+point3.x;
        cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatTop"<<endl;
        point2D temp1(xl,i);
        point2D temp2(xr,i);
        drawLine_DDA(temp1,temp2,R,G,B);
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
void drawTriangle(point2D point1,point2D point2,point2D point3,float R,float G,float B)
{
    float xtop,ytop,xmid,ymid,xtmp,xbottom,ybottom;
    if(point1.y == point2.y)
    {
        if(point3.y>=point1.y)
        {
            drawTriangle_flatBottom(point3,point1,point2,R,G,B);
        }else
        {
            drawTriangle_flatTop(point1,point2,point3,R,G,B);
        }
    }else if(point1.y == point3.y)
    {
        if(point2.y >= point1.y)
        {
            drawTriangle_flatBottom(point2,point1,point3,R,G,B);
        }else
        {
            drawTriangle_flatTop(point1,point3,point2,R,G,B);
        }
    }else if(point2.y == point3.y)
    {
        if(point1.y>=point2.y)
        {
            drawTriangle_flatBottom(point1,point2,point3,R,G,B);
        }else
        {
            drawTriangle_flatTop(point2,point3,point1,R,G,B);
        }
    }else //如果不是平顶、平底三角形
    {
        vector<pair<float,float>>vc;
        vc.push_back(make_pair(point1.x,point1.y));
        vc.push_back(make_pair(point2.x,point2.y));
        vc.push_back(make_pair(point3.x,point3.y));
        sort(vc.begin(),vc.end(),comp);
        xtop = vc[0].first;
        ytop = vc[0].second;
        xmid = vc[1].first;
        ymid = vc[1].second;
        xbottom = vc[2].first;
        ybottom = vc[2].second;
        xtmp = (ymid-ytop)*(xbottom-xtop)/(ybottom-ytop)+xtop;
        cout<<xtop<<" "<<ytop<<" "<<xmid<<" "<<ymid<<" "<<xbottom<<" "<<ybottom<<" "<<xtmp<<endl;
        point2D top(xtop,ytop);
        point2D tmp(xtmp,ymid);
        point2D mid(xmid,ymid);
        point2D bottom(xbottom,ybottom);
        drawTriangle_flatBottom(top,tmp,mid,R,G,B);
        drawTriangle_flatTop(mid,tmp,bottom,R,G,B);
    }
}
