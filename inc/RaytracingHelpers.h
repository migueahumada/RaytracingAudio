#pragma once
#include "MathObjects.h"
#include "Audio.h"

namespace ShapeType
{
  enum E {
    kEmpty = 0,
    kSphere,
    kPlane,
    kTriangle,
    kPolygon
  };
}

struct AudioProperties
{
  REAL_TYPE reflectionCoeff{ 0.5 };
};

struct IntersectionInfo
{

  Vector3 point;
  Vector3 normal;
  Color color;
  REAL_TYPE kA{ 0 };
  REAL_TYPE kD{ 0 };
  REAL_TYPE kS{ 0 };
  ShapeType::E type{ ShapeType::kEmpty };
  AudioProperties audioProperties;

};

bool RayTriangleIntersection(const Ray& ray,
  const Triangle& tri,
  REAL_TYPE& t,
  REAL_TYPE maxDepth)
{
  const Vector3 e1 = tri.v1 - tri.v0;
  const Vector3 e2 = tri.v2 - tri.v0;
  const Vector3 p = ray.direction.cross(e2);
  const REAL_TYPE det = e1.dot(p);

  REAL_TYPE epsilon = EPSILON;
  //Definicion de 

  if (std::abs(det) < epsilon)
  {
    return false;
  }

  const REAL_TYPE invDet = (REAL_TYPE)1.0 / det;

  const Vector3 tv = ray.position - tri.v0;
  REAL_TYPE u = tv.dot(p) * invDet;

  if (u < 0.0 || u > 1.0)
  {
    return false;
  }

  const Vector3 q = tv.cross(e1);
  REAL_TYPE v = ray.direction.dot(q) * invDet;
  if (v < epsilon || u + v > 1.0f)
  {
    return false;
  }

  t = e2.dot(q) * invDet;

  return t >= epsilon && t <= maxDepth;


}