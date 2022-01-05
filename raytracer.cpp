#include <iostream>
#include "ppm.h"
#include "Ray.h"
#include "Algebra.h"
#include "SceneHandler.h"
typedef unsigned char RGB[3];
using namespace parser;

int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    //parser::Scene scene;

   // scene.loadFromXml(argv[1]);

    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.
    /*
    const RGB BAR_COLOR[8] =
    {
        { 255, 255, 255 },  // 100% White
        { 255, 255,   0 },  // Yellow
        {   0, 255, 255 },  // Cyan
        {   0, 255,   0 },  // Green
        { 255,   0, 255 },  // Magenta
        { 255,   0,   0 },  // Red
        {   0,   0, 255 },  // Blue
        {   0,   0,   0 },  // Black
    };

    int width = 640, height = 480;
    int columnWidth = width / 8;

    unsigned char* image = new unsigned char [width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int colIdx = x / columnWidth;
            image[i++] = BAR_COLOR[colIdx][0];
            image[i++] = BAR_COLOR[colIdx][1];
            image[i++] = BAR_COLOR[colIdx][2];
        }
    }
    free(image);
    //swrite_ppm("/Users/selin/Desktop/477\\ HW1/git", image, width, height);
    //Vec3f r,l,v;
    //r.y = 3;
    //l.y = 4;
    //v = r + l;
    //v + r;


    // For example : from each camera call generate ray and record those rays into a vector.*/
    SceneHandler s(argv[1]);
    s.render();
}
