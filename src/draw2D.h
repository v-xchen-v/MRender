#ifndef DRAW2D_H
#define DRAW2D_H
extern const float mrender_pointSize;

class point2D
{
public:
    float x;
    float y;
    // matrixPoint* matrixP;
    point2D()
    {
    	x = 0;
    	y = 0;
    };
    point2D(float point_x,float point_y):x(point_x),y(point_y){};
};
// class Line2D
// {
// public:
//     point2D line_point1;
//     point2D line_point2;
//     Line2D()
//     {
//         line_point1(0,0);
//         line_point2(0,0);
//     }
//     Line2D(point2D point1,point2D point2)
//     {
//         line_point1(point1);
//         line_point2(point2);
//     }
// };
/*
光栅化画点
*/
void drawPoint(point2D point,float R,float G,float B,float pointSize);

/*
光栅化画线
*/
void drawLine_Print(point2D point1,point2D point2,float R,float G,float B);
void drawLine_DDA(point2D point1,point2D point2,float R,float G,float B);

/*
光栅化画三角
*/
void drawTriangle_flatTop(point2D point1,point2D point2,point2D point3,float R,float G,float B);
void drawTriangle_flatBottom(point2D point1,point2D point2,point2D point3,float R,float G,float B);
void drawTriangle(point2D point1,point2D point2,point2D point3,float R,float G,float B);

void resizer2DGraphics(point2D point1,point2D point2,point2D point3,float Sx,float Sy);
void translate2DGraphics(point2D point1,point2D point2,point2D point3,float Sx,float Sy);
void rotate2DGraphcis(point2D point1,point2D point2,point2D point3,float a);
#endif