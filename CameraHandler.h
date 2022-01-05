#pragma once
#include "Ray.h"

using namespace parser;
class CameraHandler
{
public:
	unsigned char* image;
	Camera& camera;
	Vec3f q,u,v,w,e;
	int nx, ny;
	float suConstant, svConstant;
	int pixelId = 0;
	CameraHandler(Camera& camera);
	Ray generateRay(int i, int j);
	Ray generateRay(); //not needed
	void render();
};
