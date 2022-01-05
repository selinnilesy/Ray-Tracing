#pragma once
#include "Algebra.h"

class Ray
{
public:
	Vec3f e;
	Vec3f d;
	int recursion;
	Ray();
	Ray(const Vec3f& e, const Vec3f& d);

	inline Vec3f positionT(float t);   // returns: ray'in t'ye gore x-y-z koordinatlari

    float intersect(const Sphere& s); // returns t
    float intersect(const Face& f);
	float intersect(const Vec3f& position);

	bool checkObstacle(float minDistance, float maxDistance); // Checks wheter thereis an obstacle between given distances for a ray or not

	Vec3f calculateColor(const Vec3f& intersection, const Vec3f& normal, const Material& material);//returns RGB
	Vec3f calculateColor(float minDistance);

	Vec3f calculateDiffuse(const Vec3f& intersection, const Vec3f& normal, const Material& material); //both function might be done in one function as some calculationsa are common
	Vec3f calculateSpecular(const Vec3f& intersection, const Vec3f& normal, const Material& material);
	Ray generateReflection(const Vec3f& position, const Vec3f& normal);
};

