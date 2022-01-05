//
// Created by Selin Yıldırım on 15.11.2021.
//

// All ray-object intersections, initially sending rays to pixels,
// then calculating colors and shading/reflections
// are handled in this file.

#include "Ray.h"
/* generate new ray from camera to ith&jth pixel.
 * return: generated ray.
*/
Ray generateRay(int i, int j){
    // TO DO
    Vec3f a,b;
    a.x=a.y=a.z=b.x=b.y=b.z=0;
    return Ray(a,b);
}