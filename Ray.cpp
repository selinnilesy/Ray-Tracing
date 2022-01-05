#include "Ray.h"
#include "Algebra.h"
#include "math.h"
#include <limits>
using namespace std;
using namespace parser;
Ray::Ray()
{
}

Ray::Ray(const Vec3f& e_, const Vec3f& d_):e(e_),d(d_),recursion(parser::scene.max_recursion_depth)
{
	//selincikkk:D
	normalize(d);
}

inline Vec3f Ray::positionT(float t)
{
    return e+d*t;
}

/* return -1 if does not intersect.
 *
*/
float Ray::intersect(const Sphere& s)
{
    Vec3f diff = this->e - s.center_vertex; // o - c
    float B = dotProduct(this->d,  diff ); // d.(o-c)
    float A = dotProduct(this->d , this->d ); // d.d
    float C = dotProduct(diff, diff ) - s.radius*s.radius; // (o-c).(o-c) - R^2

    float discriminant = B*B - A*C;
    if(discriminant < -1 * epsilon ) return -1;

    float t1 = (-B + sqrt(discriminant) ) / (A) ;
    float t2 = (-B - sqrt(discriminant) ) / (A) ;
    return t1<t2 ? t1 : t2;
}

/* returns -1 if the plane and the ray are perpendicular.
 * to eachother.
*/
float Ray::intersect(const Face& f)
{
	//Vec3f normd(d);
	//normalize(normd);
    float product = dotProduct( f.normal, this->d);
    if( product < epsilon && product > -1*epsilon ) {
        //LOG_ERR("perpendicular face and normal") ;
        return -1;
    }
    Vec3f a = scene.vertex_data[f.v0_id - 1 ];
    Vec3f b = scene.vertex_data[f.v1_id - 1 ];
    Vec3f c = scene.vertex_data[f.v2_id - 1 ];
    float t = (dotProduct((a - this->e),f.normal)) / product;

    // TO DO: check if the intersection is inside triangle.
    Vec3f point = e+d*t;
    // check vertex c and point on the same direction of ab
    Vec3f vp = crossProduct(point-b , a-b);
    Vec3f vc = crossProduct(c-b, a-b);
    if(dotProduct(vp, vc) + epsilon> 0){
        // check vertex a and point on the same direction of bc
        Vec3f vp_2 = crossProduct(point-c , b-c);
        Vec3f va_2 = crossProduct(a-c, b-c);
        if(dotProduct(vp_2, va_2) +epsilon > 0){
            // check vertex b and point on the same direction of ca
            Vec3f vp_3 = crossProduct(point-a , c-a);
            Vec3f vb_3 = crossProduct(b-a, c-a);
            if(dotProduct(vp_3, vb_3) +epsilon > 0){
                return t;
            }
        }
    }
    return -1;
}

float Ray::intersect(const Vec3f& position)
{
	return (e.x - position.x)/d.x;
}

/*
 * verify intersection of the ray in the proper range.
*/
bool Ray::checkObstacle(float minDistance, float maxDistance)
{
	float t ; //= std::numeric_limits<float>::max();
	for (Sphere& s : parser::scene.spheres) {
		t = intersect(s);
		if (t > minDistance && t < maxDistance) {
			return true;
		}
	}
	for (Triangle& tr : parser::scene.triangles) {
		Face& f = tr.indices;
		t = intersect(f);
		if (t > minDistance && t < maxDistance) {
			return true;
		}
	}
	for (Mesh& m : parser::scene.meshes) {
		for (Face& f : m.faces) {
			t = intersect(f);
			Vec3f p = e + d * t;
			if (t > minDistance && t < maxDistance) {
				float dp = dotProduct(d,f.normal );
				return true;
			}
		}
	}
	return false;
}



Vec3f Ray::calculateColor(const Vec3f& intersection, const Vec3f& normal, const Material& material)
{
	Vec3f ambient = hadamardProduct(scene.ambient_light, material.ambient);
	Vec3f spec = { 0,0,0 }, diffuse = { 0,0,0 }, specAdd = { 0,0,0 }, diffuseAdd = { 0,0,0 },half;
	float cos, cosNormal;
	for (PointLight& l : scene.point_lights) {
		float distance = calculateDistance(l.position, intersection);
		Ray r(intersection, l.position - intersection);
		if (!r.checkObstacle(scene.shadow_ray_epsilon, distance)) {
			cos = max((float)0.0, dotProduct(r.d, normal));
			diffuseAdd = (cos / (distance * distance)) * l.intensity;
			//diffuseAdd = hadamardProduct(diffuseAdd, material.diffuse);
			//limitColorRange(diffuseAdd);
			diffuse = diffuse + diffuseAdd;
			if (cos == 0) {
				continue;
			}
			half = r.d - d;
			normalize(half);
			cos = max((float)0.0, dotProduct(normal, half));
			specAdd = (pow(cos, material.phong_exponent) / (distance * distance)) * l.intensity;
			//specAdd = hadamardProduct(specAdd, material.specular);
			//limitColorRange(specAdd);
			spec = spec +specAdd ;
		}
	}
	diffuse = hadamardProduct(diffuse, material.diffuse);
	spec = hadamardProduct(spec, material.specular);
	//Vec3f total = ambient + spec + diffuse;
	//limitColorRange(total);
	if(this->recursion == scene.max_recursion_depth)    return ambient + spec + diffuse;
    else return diffuse + spec;
}

/* minDistance = epsilon
 * for recursion level 1, minDistance = distance of scene
 */
Vec3f Ray::calculateColor(float minDistance)
{
	float t = std::numeric_limits<float>::max(), tmpt = 0;
	Vec3f color,normal,intersection;
	color.x = scene.background_color.x;
	color.y = scene.background_color.y;
	color.z = scene.background_color.z;
	if (recursion == -1) return color;
	Ray newRay;
	Sphere* sphere = nullptr;
	Triangle* triange = nullptr;
	Mesh* mesh = nullptr;
	Face* face = nullptr;
	Material* material;
	bool intersected = false;
	for (Sphere& s : parser::scene.spheres) {
		tmpt = intersect(s);
		if (tmpt > minDistance && tmpt < t) {
			t = tmpt;
			sphere = &s;
			material = &s.material; 
			intersection = positionT(t);
			normal = (intersection - sphere->center_vertex)/sphere->radius;
			intersected = true;
		}
	}
	for (Triangle& tr : parser::scene.triangles) {
		Face& f = tr.indices;
		tmpt = intersect(f);
		if (tmpt > minDistance && tmpt < t) {
			t = tmpt;
			face = &f;
			material = &tr.material; 
			normal = face->normal;
			intersected = true;
		}
	}
	for (Mesh& m : parser::scene.meshes) {
		for (Face& f : m.faces) {
			tmpt = intersect(f);
			if (tmpt > minDistance && tmpt < t) {
				t = tmpt;
				face = &f;
				material = &m.material; 
				normal = face->normal;
				intersected = true;
			}
		}	
	}
	if (intersected == false) {
		color.x = scene.background_color.x;
		color.y = scene.background_color.y;
		color.z = scene.background_color.z;
		return color;
	}
	intersection = positionT(t);
	color = calculateColor(intersection, normal, *material);
	if (recursion != 0 && material->is_mirror ) {
		newRay = generateReflection(intersection, normal);
		Vec3f reflectionColor = newRay.calculateColor(parser::scene.shadow_ray_epsilon);
		return color + hadamardProduct(reflectionColor, material->mirror);
	}
    else return color;
}

Vec3f Ray::calculateDiffuse(const Vec3f& intersection, const Vec3f& normal, const Material& material)
{
	float cos;
	Vec3f  diffuse;
	diffuse.x = 0;
	diffuse.y = 0;
	diffuse.z = 0;
	Vec3f diffuseAdd(diffuse);
	for (PointLight& l : scene.point_lights) {
		float distance = calculateDistance(l.position, intersection);
		Ray r(intersection, l.position - intersection);
		if (!r.checkObstacle(scene.shadow_ray_epsilon, distance)) {
			cos = max((float)0.0, dotProduct(r.d, normal));
			if (cos > 1 || cos < -1 * epsilon) {
				int bp = 0;
			}
			diffuseAdd = (cos / (distance * distance)) * l.intensity;
			//diffuseAdd = hadamardProduct(diffuseAdd, material.diffuse);
			//limitColorRange(diffuseAdd);
			diffuse = diffuse + diffuseAdd;
		}
	}
	diffuse = hadamardProduct(diffuse, material.diffuse);
	//limitColorRange(diffuse);
	return diffuse;
}

Vec3f Ray::calculateSpecular(const Vec3f& intersection, const  Vec3f& normal, const Material& material)
{
	float cos;
	Vec3f spec, half;
	spec.x = 0;
	spec.y = 0;
	spec.z = 0;
	Vec3f specAdd(spec);
	for (PointLight& l : scene.point_lights) {
		float distance = calculateDistance(l.position, intersection);
		Ray r(intersection, (l.position - intersection) / distance);
		if (!r.checkObstacle(scene.shadow_ray_epsilon, distance)) {

			half = r.d - d;
			half = normalize(half);
			cos = max((float)0.0, dotProduct(normal, half));
			float cosNormal = dotProduct(normal, r.d);
			if (cosNormal < -1 * epsilon) {
				continue;
			}
			specAdd = (pow(cos,material.phong_exponent) / (distance * distance)) * l.intensity;
			//specAdd = hadamardProduct(specAdd, material.specular);
			//limitColorRange(specAdd);
			spec = spec + specAdd;
		}
	}
	spec = hadamardProduct(spec, material.specular);
	//limitColorRange(spec);
    return spec;
}

Ray Ray::generateReflection(const Vec3f& position, const Vec3f& normal)
{
	Vec3f d_reflection;
	float cos = dotProduct(normal, -1 * d);
	Ray r(position, d + 2 * cos* normal );
	r.recursion = recursion - 1;
	return r;
}
