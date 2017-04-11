#ifndef DRAW3D_H
#define DRAW3D_H
#include <vector>
using namespace std;
void drawCube_wireframe(vector<vector<float>>vertexList);
void drawCube_fillcolor(vector<vector<float>>vertexList);
void setCubeColor_fillcolor(vector<vector<float>>vertexList);
vector<vector<float>> IOProjection(vector<vector<float>>vc);
vector<vector<float>> RotatebyX(vector<vector<float>>vc,float a);
vector<vector<float>> RotatebyY(vector<vector<float>>vc,float a);
vector<vector<float>> RotatebyZ(vector<vector<float>>vc,float a);
vector<vector<float>> Rotate3D(vector<vector<float>>vc,float a,float b,float c);
#endif