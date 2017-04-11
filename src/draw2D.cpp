#include "draw2D.h"
#include "matrix.h"

#include <GLFW/glfw3.h>
#include "math.h"
#include "iostream"
#include "vector"

using namespace std;
int drawPCount =0;

//图形绘制
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
    if(R)
    {
        drawPCount++;
        // cout<<"drawPoint:"<<drawPCount<<" R:"<<R<<endl;
    }
}
void setPointColor(point2D point,float R,float G,float B,float depth)
{
    int pos_x = (int)(point.x * base) + base;
    int pos_y = (int)(point.y * base) + base;
    // cout<<pos_x<<" "<<pos_y<<" zBufferPoint"<<endl;

    //error zbp是一个局部变量，他的值函数结束后就被销毁，实际上并没有给矩阵进行复制
    // zBufferPoint zbp = zBufferMatrix[pos_x][pos_y];
    // zbp.setColor(R,G,B);
    //正确写法
    if(depth >= zBufferMatrix[pos_x][pos_y].depth)
    {
        zBufferMatrix[pos_x][pos_y].setColor(R,G,B);
        zBufferMatrix[pos_x][pos_y].setDepth(depth);
    }
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

void setLineColor(point2D point1,point2D point2,float R,float G,float B,float depth)
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
        setPointColor(tempPoint,R,G,B,depth);
        point1.x+=dx;
        point1.y+=dy;
    }
    // for(int i=0;i<base;i++)
    // {
        // for(int j=-base;j<0;j++)
        // {
        //     zBufferMatrix[j+base][j+base].G = 1;
        // }
    // }
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
        // cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatBottom"<<endl;
        point2D point1(xl,i);
        point2D point2(xr,i);
        drawLine_DDA(point1,point2,R,G,B);
    }
}

void setTriangleColor_flatBottom(point2D point1,point2D point2,point2D point3,float R,float G,float B,float depth)
{
    for(float i = point1.y; i>=point2.y ;i-=1/base)
    {
        float xl = (i-point1.y)*(point2.x-point1.x)/(point2.y-point1.y)+point1.x;
        float xr = (i-point1.y)*(point3.x-point1.x)/(point3.y-point1.y)+point1.x;
        // cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatBottom"<<endl;
        point2D point1(xl,i);
        point2D point2(xr,i);
        setLineColor(point1,point2,R,G,B,depth);
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
        // cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatTop"<<endl;
        point2D temp1(xl,i);
        point2D temp2(xr,i);
        drawLine_DDA(temp1,temp2,R,G,B);
    }
}
void setTriangleColor_flatTop(point2D point1,point2D point2,point2D point3,float R,float G,float B,float depth)
{
    for(float i = point1.y; i>=point3.y ;i-=1/base)
    {
        float xl = (i-point3.y)*(point1.x-point3.x)/(point1.y-point3.y)+point3.x;
        float xr = (i-point3.y)*(point2.x-point3.x)/(point2.y-point3.y)+point3.x;
        // cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatTop"<<endl;
        point2D temp1(xl,i);
        point2D temp2(xr,i);
        setLineColor(temp1,temp2,R,G,B,depth);
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
        // cout<<xtop<<" "<<ytop<<" "<<xmid<<" "<<ymid<<" "<<xbottom<<" "<<ybottom<<" "<<xtmp<<endl;
        point2D top(xtop,ytop);
        point2D tmp(xtmp,ymid);
        point2D mid(xmid,ymid);
        point2D bottom(xbottom,ybottom);
        drawTriangle_flatBottom(top,tmp,mid,R,G,B);
        drawTriangle_flatTop(mid,tmp,bottom,R,G,B);
    }
}

void setTriangleColor(point2D point1,point2D point2,point2D point3,float R,float G,float B,float depth)
{
    float xtop,ytop,xmid,ymid,xtmp,xbottom,ybottom;
    if(point1.y == point2.y)
    {
        if(point3.y>=point1.y)
        {
            setTriangleColor_flatBottom(point3,point1,point2,R,G,B,depth);
        }else
        {
            setTriangleColor_flatTop(point1,point2,point3,R,G,B,depth);
        }
    }else if(point1.y == point3.y)
    {
        if(point2.y >= point1.y)
        {
            setTriangleColor_flatBottom(point2,point1,point3,R,G,B,depth);
        }else
        {
            setTriangleColor_flatTop(point1,point3,point2,R,G,B,depth);
        }
    }else if(point2.y == point3.y)
    {
        if(point1.y>=point2.y)
        {
            setTriangleColor_flatBottom(point1,point2,point3,R,G,B,depth);
        }else
        {
            setTriangleColor_flatTop(point2,point3,point1,R,G,B,depth);
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
        setTriangleColor_flatBottom(top,tmp,mid,R,G,B,depth);
        setTriangleColor_flatTop(mid,tmp,bottom,R,G,B,depth);
    }
}

//图形变换

//假设是一个三角形，缩放
//两个参数Sx，Sy分别表示在X轴、Y轴上的缩放比例
void resizer2DGraphics(point2D point1,point2D point2,point2D point3,float Sx,float Sy)
{
    vector<vector<float>>arrA,arrB,result;
    matrix2D matrix2d;
    matrix2d.resizer(Sx,Sy);
    // cout<<"m2d[0]:"<<matrix2d.matrix[0][0]<<" "<<matrix2d.matrix[0][1]<<" "<<matrix2d.matrix[0][2]<<endl;
    // cout<<"m2d[1]:"<<matrix2d.matrix[1][0]<<" "<<matrix2d.matrix[1][1]<<" "<<matrix2d.matrix[1][2]<<endl;
    // cout<<"m2d[2]:"<<matrix2d.matrix[2][0]<<" "<<matrix2d.matrix[2][1]<<" "<<matrix2d.matrix[2][2]<<endl;

    // cout<<"mp[0]:"<<tmp.matrix[0][0]<<endl;
    // cout<<"mp[1]:"<<tmp.matrix[0][1]<<endl;
    // cout<<"mp[2]:"<<tmp.matrix[0][2]<<endl;
    
    // for(int i=0;i<arrA.size();i++)
    // {
    //     for(int j=0;j<arrA[0].size();j++)
    //     {
    //         cout<<arrA[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    arrB = matrix2d.getMatrixData();
    // for(int i=0;i<arrB.size();i++)
    // {
    //     for(int j=0;j<arrB[0].size();j++)
    //     {
    //         cout<<arrB[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    matrixPoint tmp(point1);
    arrA = tmp.getMatrixData();
    result = matrix_multiply(arrA,arrB);
    point2D newPoint1(result[0][0],result[0][1]);

    matrixPoint tmp2(point2);
    arrA = tmp2.getMatrixData();
    result = matrix_multiply(arrA,arrB);
    point2D newPoint2(result[0][0],result[0][1]);

    matrixPoint tmp3(point3);
    arrA = tmp3.getMatrixData();
    result = matrix_multiply(arrA,arrB);
    point2D newPoint3(result[0][0],result[0][1]);
    drawTriangle(newPoint1,newPoint2,newPoint3,0,1,0);
}

void translate2DGraphics(point2D point1,point2D point2,point2D point3,float Sx,float Sy)
{
    // matrixPoint matrix1(point1);
    // matrix1.matrix[0][0] += Sx;
    // matrix1.matrix[0][1] += Sy;
    // point1.update(matrix1);
    
    // matrixPoint matrix2(point2);
    // matrix2.matrix[0][0] += Sx;
    // matrix2.matrix[0][1] += Sy;
    // point2.update(matrix2);

    // matrixPoint matrix3(point3);
    // matrix3.matrix[0][0] += Sx;
    // matrix3.matrix[0][1] += Sy;
    // point3.update(matrix3);

    point1.x += Sx;
    point1.y += Sy;

    point2.x += Sx;
    point2.y += Sy;

    point3.x += Sx;
    point3.y += Sy;
    drawTriangle(point1,point2,point3,0,0,1);
}
/*
以逆时针为正角度
旋转a度

x' = xcosa - ysina;
y' = xsina + ycosa; 
[x' y' 1] = [x y 1][ cosa sina 0]
                   [-sina cosa 0]
                   [   0   0   1]
*/
void rotate2DGraphcis(point2D point1,point2D point2,point2D point3,float a)
{
    vector<vector<float>>arrA,arrB,result;
    matrix2D matrix2d;
    matrix2d.rotate(a);
    arrB = matrix2d.getMatrixData();
    // cout<<"m2d[0]:"<<matrix2d.matrix[0][0]<<" "<<matrix2d.matrix[0][1]<<" "<<matrix2d.matrix[0][2]<<endl;
    // cout<<"m2d[1]:"<<matrix2d.matrix[1][0]<<" "<<matrix2d.matrix[1][1]<<" "<<matrix2d.matrix[1][2]<<endl;
    // cout<<"m2d[2]:"<<matrix2d.matrix[2][0]<<" "<<matrix2d.matrix[2][1]<<" "<<matrix2d.matrix[2][2]<<endl;

    matrixPoint tmp(point1);
    arrA = tmp.getMatrixData();
    result = matrix_multiply(arrA,arrB);
    point2D newPoint1(result[0][0],result[0][1]);

    tmp.setValue(point2);
    arrA = tmp.getMatrixData();
    result = matrix_multiply(arrA,arrB);
    point2D newPoint2(result[0][0],result[0][1]);

    tmp.setValue(point3);
    arrA = tmp.getMatrixData();
    result = matrix_multiply(arrA,arrB);
    point2D newPoint3(result[0][0],result[0][1]);
    drawTriangle(newPoint1,newPoint2,newPoint3,1,0,0);
}