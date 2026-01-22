#include "Scene.h"

Scene::Scene(const Light& light, 
             const Vector<Plane>& planes, 
             const Vector<Sphere>& spheres, 
             const Vector<Triangle>& triangles, 
             const Vector3& eye) 
  : m_light(light),
    m_planes(planes),
    m_spheres(spheres),
    m_triangles(triangles),
    m_eye(eye)
{
}
