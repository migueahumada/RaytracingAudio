#pragma once


#include "tiny_obj_loader.h"
#include "MathObjects.h"

class Scene
{
public:
  Scene(const Light& light,
    const Vector<Plane>& planes,
    const Vector<Sphere>& spheres,
    const Vector<Triangle>& triangles,
    const Vector3& eye = Vector3(0, 0, 0));

  template<typename Real>
  void AddModelTriangles(const tinyobj::attrib_t& attrib,
    const Vector<tinyobj::shape_t>& shapes,
    Real kA, Real kD, Real kS)
  {
    for (size_t s = 0; s < shapes.size(); s++) {
      // Loop over faces(polygon)
      size_t index_offset = 0;

      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

        Vector3 v0, v1, v2;

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

          tinyobj::real_t x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
          tinyobj::real_t y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
          tinyobj::real_t z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

          if (v == 0)
          {
            v0 = Vector3(x, y, z);
          }
          if (v == 1)
          {
            v1 = Vector3(x, y, z);
          }
          if (v == 2)
          {
            v2 = Vector3(x, y, z);
          }
        }

        Vector3 offset(0, 0, 60);
        m_triangles.emplace_back(Triangle((v0 * 20) + offset,
          (v1 * 20) + offset,
          (v2 * 20) + offset,
          { 255,255,0 }, kA, kD, kS));

        index_offset += fv;
      }
    }
  }

public:

  Vector3 m_eye;
  Light m_light;
  Vector<Plane> m_planes;
  Vector<Sphere> m_spheres;
  Vector<Triangle> m_triangles;

};

