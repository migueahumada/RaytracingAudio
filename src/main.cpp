#include "Audio.h"
#include "Image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#ifdef _WIN32
  #define INPATH "../rsc/Woosh.wav"
  #define OUTPATH "../rsc/out2.wav"
  #define OUTPATH2 "../rsc/out3.wav"

  #define IMG_INPATH "../rsc/TestImage.bmp"
  #define IMG_OUTPATH "../rsc/OutImage.bmp"

  #define MODEL_INPATH "../rsc/chango.obj"

#endif

#ifdef __APPLE__
  #define INPATH "../../rsc/SoundFile_01.wav"
  #define OUTPATH "../../rsc/out2.wav"
  #define OUTPATH2 "../../rsc/out3.wav"

  #define IMG_INPATH "../../rsc/TestImage.bmp"
  #define IMG_OUTPATH "../../rsc/OutImage.bmp"

  #define MODEL_PATH "../../rsc/stanford-bunny.obj"
#endif

#ifdef __UNIX__
  #define INPATH "../../rsc/SoundFile_01.wav"
  #define OUTPATH "../../rsc/out2.wav"
  #define OUTPATH2  "../../rsc/out3.wav"
#endif

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

struct IntersectionInfo
{

  Vector3 point;
  Vector3 normal;
  Color color;
  REAL_TYPE kA{ 0 };
  REAL_TYPE kD{ 0 };
  REAL_TYPE kS{ 0 };
  ShapeType::E type{ ShapeType::kEmpty };


};



void audioProcesses()
{
  Audio audio;
  audio.decode(INPATH);
  audio.processAudio();

  audio.butterworth(FilterType::LOWPASS, 800.0f);
  audio.butterworth(FilterType::HIGHPASS, 100.0f);
  audio.biquad(FilterType::LOWPASS,800.0,0.707f);
  audio.biquad(FilterType::HIGHPASS, 100.0, 0.707f);

  audio.encode(OUTPATH);

  Audio newAudio;
  uint32 milliseconds = 250;
  newAudio.create(96000, 16, 4, milliseconds);
  newAudio.sine(0.5f, 440.0f);
  newAudio.encode(OUTPATH2);

}

/*
  Creates a viewport that can easily be converted into an image.
*/
class Viewport 
{
 public:

  Viewport(uint16 width, uint16 height) 
  : m_width(width), 
    m_height(height),
    m_ratio((REAL_TYPE)height/100),
    m_upperLeftCorner(Vector3(50, 50, 100))
  {

  }

  REAL_TYPE m_ratio = 0;
  uint16 m_width;
  uint16 m_height;
  Vector3 m_upperLeftCorner;

};

class Scene
{
 public:
  Scene(const Light& light,
        const Vector<Plane>& planes,
        const Vector<Sphere>& spheres,
        const Vector<Triangle>& triangles,
        const Vector3& eye = Vector3(0,0,0)) 
    : m_light(light),
      m_planes(planes),
      m_spheres(spheres),
      m_triangles(triangles),
      m_eye(eye)
  {}
  
 public:

  Vector3 m_eye;
  Light m_light;
  Vector<Plane> m_planes;
  Vector<Sphere> m_spheres;
  Vector<Triangle> m_triangles;
  
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

  if (std::abs(det) < 0.0000001)
  {
    return false;
  }

  const REAL_TYPE invDet = (REAL_TYPE)1.0/det;

  const Vector3 tv = ray.position - tri.v0;
  REAL_TYPE u = tv.dot(p) * invDet;

  if (u < 0.0 || u > 1.0)
  {
    return false;
  }

  const Vector3 q = tv.cross(e1);
  REAL_TYPE v = ray.direction.dot(q) * invDet;
  if (v < 0.0f || u + v > 1.0f)
  {
    return false;
  }

  t = e2.dot(q) * invDet;
  
  return t >= 0.000001 && t <= maxDepth;


}

IntersectionInfo findClosestIntersection(const Ray& ray,
                                        const Vector<Sphere>& spheres,
                                        const Vector<Plane>& planes,
                                        const Vector<Triangle>& triangles)
{
  IntersectionInfo intersectionInfo;

  //---------------Sphere-------------
  int closestSphereIndex = 0;
  REAL_TYPE smallestSolutionSphere = 50000;

  size_t count = spheres.size();
  Vector<REAL_TYPE> solutions;
  Vector<size_t> index;

  for (size_t i = 0; i < count; ++i)
  {
    
    REAL_TYPE a = ray.direction.dot(ray.direction);
    REAL_TYPE b = 2 * (ray.direction.dot(ray.position - spheres[i].center));
    REAL_TYPE c = (ray.position - spheres[i].center).dot(ray.position - spheres[i].center) - spheres[i].m_radius * spheres[i].m_radius;

    REAL_TYPE discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
      continue;
    }

    REAL_TYPE sqrtDiscriminant = std::sqrt(discriminant);

    REAL_TYPE t1 =(- b - sqrtDiscriminant) / (2 * a);
    if (t1 > 0.001 && t1 < 50000)
    {
      solutions.push_back(t1);
      index.push_back(i);
    }
    else
    {
      REAL_TYPE t2 = (-b + sqrtDiscriminant) / (2 * a);
      if (t2 > 0.001 && t2 < 50000)
      {
        solutions.push_back(t2);
        index.push_back(i);
      }
    }
  }

  if (!solutions.empty())
  {
    REAL_TYPE tmp = solutions[0];
    int j = 0;
    for (size_t i = 0; i < solutions.size(); ++i)
    {
      if (solutions[i] < tmp)
      {
        tmp = solutions[i];
        j = (int)i;
      }
    }

    closestSphereIndex = (int)index[j];
    smallestSolutionSphere = solutions[j];
  }

  //---------------Plane-------------
  int closestPlaneIndex = 0;
  REAL_TYPE smallestSolutionPlane = 50000;

  count = planes.size();
  solutions.clear();
  index.clear();

  for (size_t i = 0; i < count; ++i)
  {
    REAL_TYPE a = (planes[i].point - ray.position).dot(planes[i].normal);
    REAL_TYPE b = ray.direction.dot(planes[i].normal);
    REAL_TYPE t = a / b;

    if (t > 0.001 && t < 50000)
    {
      solutions.push_back(t);
      index.push_back(i);
    }
  }

  if (!solutions.empty())
  {
    REAL_TYPE tmp = solutions[0];
    int j = 0;
    for (size_t i = 0; i < solutions.size(); ++i)
    {
      if (solutions[i] < tmp)
      {
        tmp = solutions[i]; 
        j = (int)i;
      }
    }

    closestPlaneIndex = (int)index[j];
    smallestSolutionPlane = solutions[j];
  }

  //--------------Triangle------------
  int closestTriangleIndex = 0;
  REAL_TYPE smallestSolutionTriangle = 50000;

  count = triangles.size();
  solutions.clear();
  index.clear();

  for (size_t i = 0; i < count; ++i)
  {
    REAL_TYPE t;

    if (RayTriangleIntersection(ray, triangles[i], t, 50000))
    {
      solutions.push_back(t);
      index.push_back(i);
    }
    
  }

  if (!solutions.empty())
  {
    REAL_TYPE tmp = solutions[0];
    int j = 0;
    for (size_t i = 0; i < solutions.size(); ++i)
    {
      if (solutions[i] < tmp)
      {
        tmp = solutions[i];
        j = (int)i;
      }
    }

    closestTriangleIndex = (int)index[j];
    smallestSolutionTriangle = solutions[j];
  }

  //------------DepthTestings----------
  if (smallestSolutionPlane < smallestSolutionSphere && smallestSolutionPlane < smallestSolutionTriangle)
  {
    intersectionInfo.point = ray.where(smallestSolutionPlane);
    intersectionInfo.normal = planes[closestPlaneIndex].normal;
    intersectionInfo.color = planes[closestPlaneIndex].color;
    intersectionInfo.kA = planes[closestPlaneIndex].coeffs.x;
    intersectionInfo.kD = planes[closestPlaneIndex].coeffs.y;
    intersectionInfo.kS = planes[closestPlaneIndex].coeffs.z;
    intersectionInfo.type = ShapeType::kPlane;
  }
  else if(smallestSolutionSphere < smallestSolutionPlane && smallestSolutionSphere < smallestSolutionTriangle)
  {
    intersectionInfo.point = ray.where(smallestSolutionSphere);
    intersectionInfo.normal = (intersectionInfo.point - spheres[closestSphereIndex].center).getNormalized();
    intersectionInfo.color = spheres[closestSphereIndex].color;
    intersectionInfo.kA = spheres[closestSphereIndex].coeffs.x;
    intersectionInfo.kD = spheres[closestSphereIndex].coeffs.y;
    intersectionInfo.kS = spheres[closestSphereIndex].coeffs.z;
    intersectionInfo.type = ShapeType::kSphere;
  }
  else if (smallestSolutionTriangle < smallestSolutionPlane && smallestSolutionTriangle < smallestSolutionSphere)
  {
    intersectionInfo.point = ray.where(smallestSolutionTriangle);
    intersectionInfo.normal = triangles[closestTriangleIndex].normal;
    intersectionInfo.color = triangles[closestTriangleIndex].color;
    intersectionInfo.kA = triangles[closestTriangleIndex].coeffs.x;
    intersectionInfo.kD = triangles[closestTriangleIndex].coeffs.y;
    intersectionInfo.kS = triangles[closestTriangleIndex].coeffs.z;
    intersectionInfo.type = ShapeType::kTriangle;
  }
  


  return intersectionInfo;
}

Color calculateColor(const IntersectionInfo& info, 
                     const Light& light, 
                     const Vector<Sphere>& spheres,
                     const Vector<Plane>& planes,
                     const Vector<Triangle>& triangles,
                     const Vector3& currentDir)
{
  Color result;
  Color ambient, diffuse, specular, defaultColor = info.color;
  REAL_TYPE kA = info.kA;
  REAL_TYPE kD = info.kD;
  REAL_TYPE kS = info.kS;

  Vector3 relativePos = light.position - info.point;
  Vector3 lightDir = relativePos.getNormalized();
  ambient = defaultColor * light.ambientIntensity * kA;

  REAL_TYPE lightDistance = relativePos.length();
  auto attenuation = light.intensity / (lightDistance * lightDistance);
  diffuse = defaultColor * kD * attenuation * std::max((REAL_TYPE)0.f, info.normal.dot(lightDir));

  Vector3 I = lightDir * -1;
  Vector3 N = info.normal;
  Vector3 reflectDir = I - 2.f * (I.dot(N)) * N;
  specular = Color(255,255,255) * kS * attenuation * pow(std::max((REAL_TYPE)0.0, reflectDir.dot(currentDir * -1)), (REAL_TYPE)20);

  //SATURACION DE COLOR.
  result = ambient + diffuse + specular;
  result.x = std::min((REAL_TYPE)255, result.x);
  result.y = std::min((REAL_TYPE)255, result.y);
  result.z = std::min((REAL_TYPE)255, result.z);

  return result;
}

Color findColor(const Ray& ray,
                const Vector<Sphere>& spheres,
                const Vector<Plane>& planes,
                const Vector<Triangle>& triangles,
                const Light& light,
                int maxDepth = 3)
{
  Color colorResult(0, 0, 0);
  Color noIntersection(0, 0, 0);
  int depth = 0;
  Ray currentRay = ray;
  
  while (depth < maxDepth)
  {
    IntersectionInfo intersectionInfo = findClosestIntersection(currentRay, spheres, planes,triangles);

    if (intersectionInfo.type != ShapeType::kEmpty)
    {
      depth = depth + 1;
      colorResult = colorResult + calculateColor(intersectionInfo, light, spheres, planes,triangles, currentRay.direction);
      Vector3 intersectionNormal = intersectionInfo.normal;
      currentRay.direction = currentRay.direction - 2 * (currentRay.direction.dot(intersectionNormal)) * intersectionNormal;
      currentRay.position = intersectionInfo.point;
    }
    else
    {
      break;
    }

  }

  if (depth != 0)
  {
    return colorResult / (REAL_TYPE)depth;
  }
  else
  {
    return noIntersection;
  }
  

  return colorResult;
}

int main()
{
  const int width = 1920;
  const int height = 1080;

  const int AASamples = 1;

  REAL_TYPE kA = (REAL_TYPE)0.3;
  REAL_TYPE kD = (REAL_TYPE)0.4;
  REAL_TYPE kS = (REAL_TYPE)0.6;

  //Spheres
  Vector<Sphere> spheres;

  spheres.emplace_back(Vector3(-36, -28, 220), 10, Vector3(139, 0, 0), kA, kD, kS);
  spheres.emplace_back(Vector3(-55, -23, 230), 35, Vector3(255, 0, 0), kA, kD, kS);
  spheres.emplace_back(Vector3(0, 0, 107), 10, Vector3(255, 69, 0), kA, kD, kS);
  spheres.emplace_back(Vector3(13, -11, 235), 10, Vector3(255, 215, 0), kA, kD, kS);
  spheres.emplace_back(Vector3(-151, -27, 220), 14, Vector3(128, 128, 0), kA, kD, kS);

  //Planes (Room)
  Vector3 norm1(-5, 0, -4), point1(1300, 500, 500), planeColor(230, 182, 200);
  Vector3 norm2(5, 0, -4), point2(-1000, 500, 500);
  Vector3 norm3(0, 0, 1), point3(0, 0, 0);
  Vector3 normal(0, 5, -1), point(0, -60, 120);

  Plane wall1(norm1, point1, planeColor, kA, kD, kS);
  Plane wall2(norm2, point2, planeColor, kA, kD, kS);
  Plane wall3(norm3, point3, planeColor, kA, kD, kS);
  Plane floor(normal, point, planeColor, kA, kD, kS);
  
  Vector<Plane> planes
  {
    floor,
    wall1,
    wall2,
    wall3
  };

  //Triangles
  Vector3 v0(30, 23, 100);
  Vector3 v1(20, 12, 100);
  Vector3 v2(13, 26, 100);
  
  Triangle triangle(v0,v1,v2, Vector3(0,255,255),kA,kD,kS);

  Vector<Triangle> triangles;
  triangles.push_back(triangle);
  
  //Lights
  Vector3 eye(0, 0, 0);
  Vector3 lightPos(400, 400, 400);
  REAL_TYPE lightIntensity = 585000;
  REAL_TYPE ambientIntensity = 1;
  Light light(lightPos, lightIntensity, ambientIntensity);

  //Viewport
  Viewport vp(width, height);

  //Scene
  Scene scene(light, planes, spheres, triangles, eye);
  
  tinyobj::attrib_t attrib;
  Vector<tinyobj::shape_t> shapes;
  Vector<tinyobj::material_t> material;
  String err;
  String fileName = MODEL_INPATH;
  
  tinyobj::LoadObj(&attrib, &shapes, nullptr, &err, fileName.c_str(), nullptr, true);

  // Loop over shapes
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
      
      Vector3 offset(0,0,60);
      scene.m_triangles.emplace_back(Triangle((v0*20) + offset , 
                                              (v1*20) + offset ,
                                              (v2*20) + offset , 
                                              { 255,255,0 }, kA, kD, kS));

      index_offset += fv;
    }
  }


  //Image creation
  Image image;
  image.create(vp.m_width, vp.m_height,32);

  //RandomEngine
  RandomEngine<REAL_TYPE> randomEngine;

  //Image processing
  for (int y = 0; y < vp.m_height; ++y)
  {
    for (int x = 0; x < vp.m_width; ++x)
    {
      Color pixelColor;

      for (size_t j = 1; j <= AASamples; ++j)
      {
        float dx = (float)randomEngine.getNumber();
        float dy = (float)randomEngine.getNumber();
        
        Vector3 tmpPixel((-x + dx) / vp.m_ratio, (-y + dy) / vp.m_ratio, 0);
        Vector3 pixel = vp.m_upperLeftCorner + tmpPixel;

        Ray currentRay(scene.m_eye, pixel - scene.m_eye);
        pixelColor = pixelColor + findColor(currentRay, scene.m_spheres, scene.m_planes, scene.m_triangles, scene.m_light, 2);
      }
      Color_BMP colorbmp;

      colorbmp.r = (unsigned char)std::min((REAL_TYPE)255, pixelColor.x / (REAL_TYPE)AASamples);
      colorbmp.g = (unsigned char)std::min((REAL_TYPE)255, pixelColor.y / (REAL_TYPE)AASamples);
      colorbmp.b = (unsigned char)std::min((REAL_TYPE)255, pixelColor.z / (REAL_TYPE)AASamples);
      colorbmp.a = 255;
      image.setPixel(x, y, colorbmp);
    }
  }
  
  image.encode(IMG_OUTPATH);

  return 0;
}
