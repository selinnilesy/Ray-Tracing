#include "SceneHandler.h"
using namespace parser;
Scene parser::scene;
float parser::epsilon;
SceneHandler::SceneHandler(string input)
{
    scene.loadFromXml(input);
    epsilon = scene.shadow_ray_epsilon * 0.0000000000005;
	for (Camera& c : scene.cameras) {
		cameras.push_back(CameraHandler(c));
	}
    for (Sphere& s : scene.spheres) {
        s.center_vertex = scene.vertex_data[s.center_vertex_id - 1];
        s.material = scene.materials[s.material_id - 1];
    }
    for (Triangle& t : scene.triangles) {
        t.material = scene.materials[t.material_id - 1];

        Vec3f a = parser::scene.vertex_data[ t.indices.v0_id - 1];
        Vec3f b = parser::scene.vertex_data[t.indices.v1_id - 1];
        Vec3f c = parser::scene.vertex_data[t.indices.v2_id - 1];

        t.indices.normal = crossProduct((c-b), (a-b));
        normalize(t.indices.normal);
    }
    for (Mesh& m : scene.meshes) {
        m.material = scene.materials[m.material_id - 1];
        for(Face &f : m.faces){
            Vec3f a = parser::scene.vertex_data[ f.v0_id - 1];
            Vec3f b = parser::scene.vertex_data[ f.v1_id - 1];
            Vec3f c = parser::scene.vertex_data[ f.v2_id - 1];

            f.normal = crossProduct((c-b), (a-b));
            normalize(f.normal);
        }
    }
}

void SceneHandler::render()
{
	for (CameraHandler& c : cameras) c.render();
}
