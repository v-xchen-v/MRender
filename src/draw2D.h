#ifndef DRAW2D_H
#define DRAW2D_H
void drawPoint(float x,float y,float R,float G,float B);
void drawLine_Print(float x1,float y1,float x2,float y2,float R,float G,float B);
void drawLine_DDA(float x1,float y1,float x2,float y2,float R,float G,float B);
void drawTriangle_flatTop(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B);
void drawTriangle_flatBottom(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B);
void drawTriangle(float x1,float y1,float x2,float y2,float x3,float y3,float R,float G,float B);
#endif