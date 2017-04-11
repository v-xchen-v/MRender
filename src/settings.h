#ifndef SETTINGS_H
#define SETTINGS_H
const float mrender_pointSize = 2.0;
const float base = 500;
class zBufferPoint
{
public:
	float R;
	float G;
	float B;
	float depth;
	zBufferPoint():R(0),G(0),B(0),depth(-1){};//(0,0,0)背景色-黑色 深度初始化为最深
	zBufferPoint(float r,float g,float b,float d):R(r),G(g),B(b),depth(d){};
	void setColor(float r,float g,float b)
	{
		R = r;
		G = g;
		B = b;
	};
	void setDepth(float d)
	{
		depth = d;
	};
};
#endif
