#pragma once

constexpr float PI = 3.14159265f;

template<typename Real>
class Vector3_T {

public:

  Vector3_T(Real _x = 0.0f, Real _y = 0.0f, Real _z = 0.0f) :
    x(_x), y(_y), z(_z) {
  }

  inline Vector3_T operator+(const Vector3_T& other) const
  {
    return Vector3_T
    {
      x + other.x,
      y + other.y,
      z + other.z 
    };
  }

  inline Vector3_T operator-(const Vector3_T& other) const
  {
    return Vector3_T
    {
      x - other.x,
      y - other.y,
      z - other.z
    };
  }

  inline Vector3_T operator*(Real scalar) const
  {
    return Vector3_T
    {
      x * scalar,
      y * scalar,
      z * scalar
    };
  }

  inline Vector3_T operator/(Real scalar) const
  {
    return Vector3_T
    { x / scalar,
      y / scalar,
      z / scalar
    };
  }

  NODISCARD 
  inline Real length() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  NODISCARD 
  inline Vector3_T getNormalized() const
  {

    return Vector3_T
    { x / length(),
      y / length(),
      z / length()
    };
  }

  Vector3_T normalize()
  {
    
    this->x /= length();
    this->y /= length();
    this->z /= length();
  }

  NODISCARD 
  inline Real dot(const Vector3_T& other) const
  {
    return (x * other.x) + (y * other.y) + (z * other.z);
  }

  NODISCARD 
  inline Vector3_T cross(const Vector3_T& other) const
  {
    return Vector3_T
    { 
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    };
  }

public:

  Real x = 0, y = 0, z = 0;
};

template<typename Real>
Vector3_T<Real> operator*(Real scalar, const Vector3_T<Real>& vec)
{
  Vector3_T<Real> result;
  result = vec * scalar;
  return result;
}

template<typename Real>
Vector3_T<Real> operator/(Real scalar, const Vector3_T<Real>& vec)
{
  Vector3_T<Real> result;
  result = vec / scalar;
  return result;
}

/*
*   Geometric abstraction that has:
*   - Normal
*   - Position
*   
*/
template<typename Real>
class Plane_T
{
 public:
  Plane_T() = default;
  Plane_T(const Vector3_T<Real>& normal, 
          const Vector3_T<Real>& point,
          const Vector3_T<Real>& color,
          Real kA, Real kD, Real kS) 
            : point(point),
              normal(point.getNormalized()),
              color(color),
              coeffs(kA,kD,kS)
  {}
 
 public:
   Vector3_T<Real> normal;
   Vector3_T<Real> point;
   Vector3_T<Real> color;
   Vector3_T<Real> coeffs;

};

template<typename Real>
class Ray_T {
 public:
  Ray_T() = default;
  Ray_T(const Vector3_T<Real>& position,
        const Vector3_T<Real>& direction)
        : position(position),
          direction(direction.getNormalized())
  {}

  Vector3_T<Real> where(Real t) const
  {
    return position + direction * t;
  }

 public:
  Vector3_T<Real> position;
  Vector3_T<Real> direction;
};

/*
  A Sphere can be defined implicitly as: 
    (x - xc)^2 + (y - yc)^2 +(z - zc)^2 - r^2 = 0
  
  But it can also be represented as having:
  - A position at the center (C)
  - A position to the surface of the sphere (R)
  - A radius (a)

  So that we can get the mangitude of the CR vector is a
    ||CR|| = a

  The C position never changes but the R does, so 
  we can express R as a changing vector.

    ||R - C|| = a -> VECTOR EQUATION OF A SPHERE

  Now, we can despejarlo

    || R - C || - a = 0

  Doing the dot product of a vector by itself gives us its magnitude squared.

   (||R - C||)^2 = (R - C) • (R - C) 

  So we can square the vector equation

    (||R-C||)^2 - a^2 = 0

  Which means having this:

  ---------------------------
  (R - C) • (R - C) - a^2 = 0
  ---------------------------

*/

template <typename Real>
class Sphere_T
{
 public:
  Sphere_T() = default;
  Sphere_T(const Vector3_T<Real>& center,
           Real radius,
           const Vector3_T<Real>& color,
           Real kA,
           Real kD,
           Real kS) 
   : m_center(center),
     m_radius(radius),
     m_color(color),
     m_coeffs(kA,kD,kS){}

 public:
  Vector3_T<Real> m_center;
  Vector3_T<Real> m_color;
  Vector3_T<Real> m_coeffs;
  Real m_radius = 0;

};

template<typename Real>
class Light_T 
{
 public:
  Light_T() = default;
  Light_T(const Vector3_T<Real>& position,
          Real intensity,
          Real ambientIntensity)
          : position(position),
            intensity(intensity),
            ambientIntensity(ambientIntensity)
  {}
 public:
  Vector3_T<Real> position;
  Real intensity;
  Real ambientIntensity;
};




#define REAL_TYPE float

using Vector3 = Vector3_T<REAL_TYPE>;
using Color = Vector3_T<REAL_TYPE>;
using Ray = Ray_T<REAL_TYPE>;
using Plane = Plane_T<REAL_TYPE>;
using Light = Light_T<REAL_TYPE>;
using Sphere = Sphere_T<REAL_TYPE>;

