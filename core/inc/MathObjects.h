#pragma once
#include "Prerequisites.h"

constexpr REAL_TYPE PI = 3.14159265;

template<typename T, uint32 N>
class Matrix;

inline REAL_TYPE RealCosine(REAL_TYPE number)
{
#if PRECISION_TYPE == 0
  return std::cosf(number);

#else
  return std::cos(number);
#endif // PRECISION_TYPE 0

}

inline REAL_TYPE RealSine(REAL_TYPE number)
{
#if PRECISION_TYPE == 0
  return std::sinf(number);
#else
  return std::sin(number);
#endif // PRECISION_TYPE 0

}

namespace AXIS{
  enum E {
    kX,
    kY,
    kZ,
  };
}


template<typename Real>
static Real ToDegrees(Real radians)
{
  Real degrees = radians * (Real)180 / PI;
  return degrees;
  
}

template<typename Real>
static Real ToRadians(Real degrees)
{
  Real radians = PI/(Real)180.0 * degrees;
  return radians;
}

template<typename Real>
class Vector3_T {

public:

  Vector3_T(Real _x = 0.0f, Real _y = 0.0f, Real _z = 0.0f) :
    x(_x), y(_y), z(_z) 
  {}

  Vector3_T getRotated(float angle, AXIS::E axis)
  {
    Vector3_T<Real> result;

    switch (axis)
    {
    case AXIS::kX:
      result = GetXRotationMatrix(angle) * (*this);
      break;
    case AXIS::kY:
      result = GetYRotationMatrix(angle) * (*this);
      break;
    case AXIS::kZ:
      result = GetZRotationMatrix(angle) * (*this);
      break;
    default:
      
      break;
    }
    

    return result;
  }

  void Rotate(float angle, AXIS::E axis)
  {

    switch (axis)
    {
    case AXIS::kX:
      (*this) = GetXRotationMatrix(angle) * (*this);
      break;
    case AXIS::kY:
      (*this) = GetYRotationMatrix(angle) * (*this);
      break;
    case AXIS::kZ:
      (*this) = GetZRotationMatrix(angle) * (*this);
      break;
    default:

      break;
    }
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

template <typename T, uint32 N>
class Matrix {
public:
  Matrix()
  {
    static_assert(std::is_integral_v<T> == true);
    Identity();
  }

  void Identity()
  {
    for (T i = 0; i < N; ++i)
    {
      for (T j = 0; j < N; ++j)
      {
        if (i == j)
        {
          m[i][j] = 1.0f;
          continue;
        }
        m[i][j] = 0.0f;

      }
    }
  }

  float m[N][N];
};

/*
* Row Major
*            
* | x | y | z |
* | x | y | z |
* | x | y | z |
* 
*/
template<>
class Matrix<REAL_TYPE, 3>
{
 public:
  Matrix()
  {
    Identity();
  }

  void Identity()
  {
    for (size_t i = 0; i < 3; ++i)
    {
      for (size_t j = 0; j < 3; ++j)
      {
        if (i == j)
        {
          m[i][j] = static_cast<REAL_TYPE>(1);
          continue;
        }
        m[i][j] = static_cast<REAL_TYPE>(0);

      }
    }
  }

  
  /*
  * Row Major
  *
  * | x | y | z |   
  * | x | y | z | * | x | y | z |
  * | x | y | z |   
  *
  */
  Vector3_T<REAL_TYPE> operator*(const Vector3_T<REAL_TYPE>& v)
  {
    return Vector3_T<REAL_TYPE>
    {
      m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
      m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
      m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z,
    };
  }

  REAL_TYPE m[3][3];
};

//Row Major
static Matrix<REAL_TYPE, 3> GetXRotationMatrix(REAL_TYPE angle)
{
    Matrix<REAL_TYPE,3> resultMatrix;
    
    resultMatrix.m[0][0] = 1; 
    resultMatrix.m[0][1] = 0;                 
    resultMatrix.m[0][2] = 0;

    resultMatrix.m[1][0] = 0;
    resultMatrix.m[1][1] = RealCosine(angle);
    resultMatrix.m[1][2] = -RealSine(angle);

    resultMatrix.m[2][0] = 0;
    resultMatrix.m[2][1] = RealSine(angle);
    resultMatrix.m[2][2] = RealCosine(angle);

    return resultMatrix;
}

static Matrix<REAL_TYPE, 3> GetYRotationMatrix(REAL_TYPE angle)
{
  Matrix<REAL_TYPE, 3> resultMatrix;

  resultMatrix.m[0][0] = RealCosine(angle); 
  resultMatrix.m[0][1] = 0;                 
  resultMatrix.m[0][2] = RealSine(angle);
  
  resultMatrix.m[1][0] = 0;
  resultMatrix.m[1][1] = 1;
  resultMatrix.m[1][2] = 0;

  resultMatrix.m[2][0] = -RealSine(angle);
  resultMatrix.m[2][1] = 0;
  resultMatrix.m[2][2] = RealCosine(angle);

  return resultMatrix;
}

static Matrix<REAL_TYPE, 3> GetZRotationMatrix(REAL_TYPE angle)
{
  Matrix<REAL_TYPE, 3> resultMatrix;

  resultMatrix.m[0][0] = RealCosine(angle);
  resultMatrix.m[0][1] = -RealSine(angle);
  resultMatrix.m[0][2] = 0;

  resultMatrix.m[1][0] = RealSine(angle);
  resultMatrix.m[1][1] = RealCosine(angle);
  resultMatrix.m[1][2] = 0;

  resultMatrix.m[2][0] = 0;
  resultMatrix.m[2][1] = 0;
  resultMatrix.m[2][2] = 1;

  return resultMatrix;
}

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
              normal(normal.getNormalized()),
              color(color),
              coeffs(kA,kD,kS)
  {}
 
 public:
   Vector3_T<Real> normal;
   Vector3_T<Real> point;
   Vector3_T<Real> color;
   Vector3_T<Real> coeffs;

};

template <typename Real>
class Line_T{
 public:
  Line_T() = default;
  Line_T(const Vector3_T<Real>& origin,
         const Vector3_T<Real>& end,
         const Vector3_T<Real>& color)
    : origin(origin), end(end), color(color)
  {}

 public:
  Vector3_T<Real> origin;
  Vector3_T<Real> end;
  Vector3_T<Real> color;
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
   : center(center),
     m_radius(radius),
     color(color),
     coeffs(kA,kD,kS){}

 public:
  Vector3_T<Real> center;
  Vector3_T<Real> color;
  Vector3_T<Real> coeffs;
  Real m_radius = 0;

};

template <typename Real>
class Triangle_T
{
 public:
  Triangle_T() = default;
  Triangle_T(const Vector3_T<Real>& v0,
             const Vector3_T<Real>& v1,
             const Vector3_T<Real>& v2,
             const Vector3_T<Real>& color,
             Real kA, Real kD, Real kS) 
    : v0(v0), v1(v1), v2(v2),
      normal((v1 - v0).cross(v2 - v0).getNormalized()),
      color(color),
      coeffs(kA, kD, kS),
      centroid((v0 + v1 + v2) / Real(3))
  {}

  bool isInside(const Vector3_T<Real>& point)
  {
    if (((v1 - v0).cross((point - v0))).dot((v1 - v0).cross((centroid - v0))) < 0.0000001)
    {
      return false;
    }

    if (((v2 - v0).cross((point - v0))).dot((v2 - v0).cross((centroid - v0))) < 0.0000001)
    {
      return false;
    }

    if (((v1 - v2).cross((point - v2))).dot((v1 - v2).cross((centroid - v2))) < 0.0000001)
    {
      return false;
    }

    return true;
  }
  
  Vector3_T<Real> v0;
  Vector3_T<Real> v1;
  Vector3_T<Real> v2;
  Vector3_T<Real> normal;
  Vector3_T<Real> color;
  Vector3_T<Real> coeffs;
  Vector3_T<Real> centroid;
  
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

using Vector3 = Vector3_T<REAL_TYPE>;
using Color = Vector3_T<REAL_TYPE>;
using Ray = Ray_T<REAL_TYPE>;
using Plane = Plane_T<REAL_TYPE>;
using Light = Light_T<REAL_TYPE>;
using Triangle = Triangle_T<REAL_TYPE>;
using Sphere = Sphere_T<REAL_TYPE>;
using Line = Line_T<REAL_TYPE>;
using Matrix3x3 = Matrix<REAL_TYPE, 3>;
