#include "CameraHandler.h"
#include "ppm.h"
#include <iostream>
#include "math.h"
#include <chrono>
#include <future>

using namespace std;
CameraHandler::CameraHandler(Camera& cam) : camera(cam)
{
	w = -1 * camera.gaze;
	v = camera.up;
	u = crossProduct(v, w);
	float left, right, top, bottom;
	left = cam.near_plane.x;
	right = cam.near_plane.y;
	bottom = cam.near_plane.z;
	top = cam.near_plane.w;
	Vec3f m = cam.position + (cam.gaze * cam.near_distance);
	q = m + u * left + v * top;
	suConstant = (right - left) / cam.image_width;
	svConstant = (top - bottom) / cam.image_height;
	e = cam.position;
	nx = cam.image_width;
	ny = cam.image_height;
	image = new unsigned char[nx*ny*3];
}

Ray CameraHandler::generateRay(int i, int j)
{
	float su, sv;
	Vec3f s;
	su = (i + 0.5) * suConstant;
	sv = (j + 0.5) * svConstant;
	s = q + u * su - v * sv;
	Ray ray(e, s - e);
	return ray;
}

Ray CameraHandler::generateRay()
{
	
	return Ray();
}

void CameraHandler::render()
{
	auto t_start = std::chrono::high_resolution_clock::now();
	
	cout << "rendering for file: " << camera.image_name<< " started at time" << endl;
	Ray r1, r2, r3, r4;
	int i,j;
	for ( j = 0; j < ny; j++) {
		for ( i = 0; i < nx-3; i+=4) {
			/*if ( (i == 0) && (((j*100) % nx/100*100) == 0)) {
				auto t_end = std::chrono::high_resolution_clock::now();
				double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
				cout << "rendering... %" << ((j * 100) / (nx / 100 * 100)) << " completed elapsed time: "<<(int) elapsed_time_ms/60/1000 << " minutes "<< ((int)elapsed_time_ms/1000%60)<<" seconds " << endl;
			}*/
			r1 = generateRay(i, j);
            r2 = generateRay(i+1, j);
            r3 = generateRay(i+2, j);
            r4 = generateRay(i+3, j);

            auto rgbf_1_handle = std::async(std::launch::async,static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor),  &r1,camera.near_distance);
            auto rgbf_2_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor),  &r2,camera.near_distance);
            auto rgbf_3_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor),  &r3,camera.near_distance);
            auto rgbf_4_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor),  &r4,camera.near_distance);

            Vec3f rgbf_1 = rgbf_1_handle.get();

			if (rgbf_1.x < 0 || rgbf_1.x > 255 || rgbf_1.y < 0 || rgbf_1.y > 255 || rgbf_1.z < 0 || rgbf_1.z > 255) {
                rgbf_1.x = min(255, max(0,(int) rgbf_1.x));
                rgbf_1.y = min(255, max(0, (int)rgbf_1.y));
                rgbf_1.z = min(255, max(0, (int)rgbf_1.z));
			}
			// min max might be checked if neccessary if not checked in calculate color
			image[(nx * j + i)*3] = rgbf_1.x;
			image[(nx * j + i)*3 + 1] = rgbf_1.y;
			image[(nx * j + i)*3 + 2] = rgbf_1.z;

            Vec3f rgbf_2 = rgbf_2_handle.get();

            if (rgbf_2.x < 0 || rgbf_2.x > 255 || rgbf_2.y < 0 || rgbf_2.y > 255 || rgbf_2.z < 0 || rgbf_2.z > 255) {
                rgbf_2.x = min(255, max(0,(int) rgbf_2.x));
                rgbf_2.y = min(255, max(0, (int)rgbf_2.y));
                rgbf_2.z = min(255, max(0, (int)rgbf_2.z));
            }
            // min max might be checked if neccessary if not checked in calculate color
            image[(nx * j + i + 1)*3] = rgbf_2.x;
            image[(nx * j + i + 1)*3 + 1] = rgbf_2.y;
            image[(nx * j + i + 1 )*3 + 2] = rgbf_2.z;

            Vec3f rgbf_3 = rgbf_3_handle.get();

            if (rgbf_3.x < 0 || rgbf_3.x > 255 || rgbf_3.y < 0 || rgbf_3.y > 255 || rgbf_3.z < 0 || rgbf_3.z > 255) {
                rgbf_3.x = min(255, max(0,(int) rgbf_3.x));
                rgbf_3.y = min(255, max(0, (int)rgbf_3.y));
                rgbf_3.z = min(255, max(0, (int)rgbf_3.z));
            }
            // min max might be checked if neccessary if not checked in calculate color
            image[(nx * j + i + 2)*3] = rgbf_3.x;
            image[(nx * j + i + 2)*3 + 1] = rgbf_3.y;
            image[(nx * j + i + 2 )*3 + 2] = rgbf_3.z;

            Vec3f rgbf_4 = rgbf_4_handle.get();

            if (rgbf_4.x < 0 || rgbf_4.x > 255 || rgbf_4.y < 0 || rgbf_4.y > 255 || rgbf_4.z < 0 || rgbf_4.z > 255) {
                rgbf_4.x = min(255, max(0,(int) rgbf_4.x));
                rgbf_4.y = min(255, max(0, (int)rgbf_4.y));
                rgbf_4.z = min(255, max(0, (int)rgbf_4.z));
            }
            // min max might be checked if neccessary if not checked in calculate color
            image[(nx * j + i + 3)*3] = rgbf_4.x;
            image[(nx * j + i + 3)*3 + 1] = rgbf_4.y;
            image[(nx * j + i + 3 )*3 + 2] = rgbf_4.z;
		}
		if(i<nx){
		    // process remaining ones here
		    if(nx%4==1){
                r1 = generateRay(i, j);
                auto rgbf_1_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor),  &r1,camera.near_distance);
                Vec3f rgbf_1 = rgbf_1_handle.get();

                if (rgbf_1.x < 0 || rgbf_1.x > 255 || rgbf_1.y < 0 || rgbf_1.y > 255 || rgbf_1.z < 0 || rgbf_1.z > 255) {
                    rgbf_1.x = min(255, max(0,(int) rgbf_1.x));
                    rgbf_1.y = min(255, max(0, (int)rgbf_1.y));
                    rgbf_1.z = min(255, max(0, (int)rgbf_1.z));
                }
                // min max might be checked if neccessary if not checked in calculate color
                image[(nx * j + i)*3] = rgbf_1.x;
                image[(nx * j + i)*3 + 1] = rgbf_1.y;
                image[(nx * j + i)*3 + 2] = rgbf_1.z;
		    }
		    else if(nx%4==2){
                r1 = generateRay(i, j);
                r2 = generateRay(i+1, j);
                auto rgbf_1_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor),  &r1,camera.near_distance);
                auto rgbf_2_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor),  &r2,camera.near_distance);

                Vec3f rgbf_1 = rgbf_1_handle.get();

                if (rgbf_1.x < 0 || rgbf_1.x > 255 || rgbf_1.y < 0 || rgbf_1.y > 255 || rgbf_1.z < 0 || rgbf_1.z > 255) {
                    rgbf_1.x = min(255, max(0,(int) rgbf_1.x));
                    rgbf_1.y = min(255, max(0, (int)rgbf_1.y));
                    rgbf_1.z = min(255, max(0, (int)rgbf_1.z));
                }
                // min max might be checked if neccessary if not checked in calculate color
                image[(nx * j + i)*3] = rgbf_1.x;
                image[(nx * j + i)*3 + 1] = rgbf_1.y;
                image[(nx * j + i)*3 + 2] = rgbf_1.z;

                Vec3f rgbf_2 = rgbf_2_handle.get();

                if (rgbf_2.x < 0 || rgbf_2.x > 255 || rgbf_2.y < 0 || rgbf_2.y > 255 || rgbf_2.z < 0 || rgbf_2.z > 255) {
                    rgbf_2.x = min(255, max(0,(int) rgbf_2.x));
                    rgbf_2.y = min(255, max(0, (int)rgbf_2.y));
                    rgbf_2.z = min(255, max(0, (int)rgbf_2.z));
                }
                // min max might be checked if neccessary if not checked in calculate color
                image[(nx * j + i + 1)*3] = rgbf_2.x;
                image[(nx * j + i + 1)*3 + 1] = rgbf_2.y;
                image[(nx * j + i + 1 )*3 + 2] = rgbf_2.z;

		    }
            else if(nx%4==3){ // process the last [i]th [i+1]st [i+2]nd elements.
                r1 = generateRay(i, j);
                r2 = generateRay(i+1, j);
                r3 = generateRay(i+2, j);
                auto rgbf_1_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor), &r1, camera.near_distance);
                auto rgbf_2_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor), &r2, camera.near_distance);
                auto rgbf_3_handle = std::async(std::launch::async, static_cast<Vec3f(Ray::*)(float)>(&Ray::calculateColor), &r3, camera.near_distance);

                Vec3f rgbf_1 = rgbf_1_handle.get();

                if (rgbf_1.x < 0 || rgbf_1.x > 255 || rgbf_1.y < 0 || rgbf_1.y > 255 || rgbf_1.z < 0 || rgbf_1.z > 255) {
                    rgbf_1.x = min(255, max(0,(int) rgbf_1.x));
                    rgbf_1.y = min(255, max(0, (int)rgbf_1.y));
                    rgbf_1.z = min(255, max(0, (int)rgbf_1.z));
                }
                // min max might be checked if neccessary if not checked in calculate color
                image[(nx * j + i)*3] = rgbf_1.x;
                image[(nx * j + i)*3 + 1] = rgbf_1.y;
                image[(nx * j + i)*3 + 2] = rgbf_1.z;

                Vec3f rgbf_2 = rgbf_2_handle.get();

                if (rgbf_2.x < 0 || rgbf_2.x > 255 || rgbf_2.y < 0 || rgbf_2.y > 255 || rgbf_2.z < 0 || rgbf_2.z > 255) {
                    rgbf_2.x = min(255, max(0,(int) rgbf_2.x));
                    rgbf_2.y = min(255, max(0, (int)rgbf_2.y));
                    rgbf_2.z = min(255, max(0, (int)rgbf_2.z));
                }
                // min max might be checked if neccessary if not checked in calculate color
                image[(nx * j + i + 1)*3] = rgbf_2.x;
                image[(nx * j + i + 1)*3 + 1] = rgbf_2.y;
                image[(nx * j + i + 1 )*3 + 2] = rgbf_2.z;

                Vec3f rgbf_3 = rgbf_3_handle.get();

                if (rgbf_3.x < 0 || rgbf_3.x > 255 || rgbf_3.y < 0 || rgbf_3.y > 255 || rgbf_3.z < 0 || rgbf_3.z > 255) {
                    rgbf_3.x = min(255, max(0,(int) rgbf_3.x));
                    rgbf_3.y = min(255, max(0, (int)rgbf_3.y));
                    rgbf_3.z = min(255, max(0, (int)rgbf_3.z));
                }
                // min max might be checked if neccessary if not checked in calculate color
                image[(nx * j + i + 2)*3] = rgbf_3.x;
                image[(nx * j + i + 2)*3 + 1] = rgbf_3.y;
                image[(nx * j + i + 2 )*3 + 2] = rgbf_3.z;

            }

		}
	}
	write_ppm(camera.image_name.c_str(),(unsigned char*) image, camera.image_width, camera.image_height);
}

