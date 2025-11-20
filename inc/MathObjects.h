#pragma once

constexpr float PI = 3.14159265f;

template<typename Real>
class Vector3_T {

public:
  Vector3_T() = default;

  Vector3_T(Real _x = 0.0f, Real _y = 0.0f, Real _z = 0.0f) :
    x(_x), y(_y), z(_z) {
  }

  inline Vector3_T operator+(const Vector3_T& other) const
  {
    return Vector3_T(x + other.x,
      y + other.y,
      z + other.z);
  }

  inline Vector3_T operator-(const Vector3_T& other) const
  {
    return Vector3_T(x - other.x,
      y - other.y,
      z - other.z);
  }

  inline Vector3_T operator*(Real scalar) const
  {
    return Vector3_T(x * scalar,
      y * scalar,
      z * scalar);
  }

  inline Vector3_T operator/(Real scalar) const
  {
    return Vector3_T(x / scalar,
      y / scalar,
      z / scalar);
  }

  NODISCARD 
  inline Real length() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  NODISCARD 
  inline Vector3_T getNormalized() const
  {

    return  Vector3_T(x / length(),
                      y / length(),
                      z / length());
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
    return Vector3_T(y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x);
  }

private:

  Real x, y, z = 0;
};

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