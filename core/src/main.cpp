#define TINYOBJLOADER_IMPLEMENTATION
#include "Audio.h"
#include "Image.h"
#include "RaytracingHelpers.h"
#include "Scene.h"
#include "Viewport.h"
#include "DelayLine.h"

//#include "ComputeAPI.h"

#ifdef _WIN32
  #define INPATH "../rsc/SoundFile_01.wav"
  #define OUTPATH "../rsc/OutputSound.wav"
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

// TODO:
// https://easings.net/
//Canal de una textura
// 0 - 1
//Poner un lambda para cambiar la interpolaci�n
// Callback y lambda

static const int width = 1920;
static const int height = 1080;

static const int AASamples = 1;

static constexpr REAL_TYPE kA = (REAL_TYPE)0.5;
static constexpr REAL_TYPE kD = (REAL_TYPE)0.4;
static constexpr REAL_TYPE kS = (REAL_TYPE)0.7;



IntersectionInfo findClosestIntersection(const Ray& ray,
                                        const Vector<Sphere>& spheres,
                                        const Vector<Plane>& planes,
                                        const Vector<Triangle>& triangles,
                                        const Vector<Line>& lines)
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

  //---------------Line----------------------
  /*
    v1 = o - a
    v2 = b - a
    v3 = (-dy,dx)
    
  */
  int closestLineIndex = 0;
  REAL_TYPE smallestSolutionLine = 50000;

  count = lines.size();
  solutions.clear();
  index.clear();



  for (size_t i = 0; i < count; ++i)
  {
    
    const Vector3 v1 = ray.position - lines[i].origin;
    const Vector3 v2 = lines[i].end - lines[i].origin;
    const Vector3 v3 = { -ray.direction.y ,ray.direction.x };
    


    REAL_TYPE t1 = (v2.cross(v1)).length() / v2.dot(v3);
    REAL_TYPE t2 = v1.dot(v3) / v2.dot(v3);

    if (t1 >= 0.0f && t2 >= 0.0f && t2 <= 1.0f)
    {
      solutions.push_back(t2);
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

    closestLineIndex = (int)index[j];
    smallestSolutionLine = solutions[j];
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

  auto comp = [](const Solution<REAL_TYPE>& a, const Solution<REAL_TYPE>& b)
  {
      return a.smallestSolution < b.smallestSolution;
  };

  Vector<Solution<REAL_TYPE>> vSolutions;

  vSolutions.emplace_back(closestPlaneIndex, smallestSolutionPlane, ShapeType::kPlane);
  vSolutions.emplace_back(closestSphereIndex, smallestSolutionSphere, ShapeType::kSphere);
  vSolutions.emplace_back(closestTriangleIndex, smallestSolutionTriangle, ShapeType::kTriangle);

  std::sort(vSolutions.begin(), vSolutions.end(), [](const Solution<REAL_TYPE>& a, const Solution<REAL_TYPE>& b)
  {
    return a.smallestSolution < b.smallestSolution;
  });

  Solution bestSolution = vSolutions[0];

  switch (bestSolution.shapeType)
  {
    case ShapeType::kSphere:
      intersectionInfo.point = ray.where(bestSolution.smallestSolution);
      intersectionInfo.normal = (intersectionInfo.point - spheres[bestSolution.index].center).getNormalized();
      intersectionInfo.color = spheres[bestSolution.index].color;
      intersectionInfo.kA = spheres[bestSolution.index].coeffs.x;
      intersectionInfo.kD = spheres[bestSolution.index].coeffs.y;
      intersectionInfo.kS = spheres[bestSolution.index].coeffs.z;
      intersectionInfo.type = ShapeType::kSphere;
      break;
    case ShapeType::kPlane:
      intersectionInfo.point = ray.where(bestSolution.smallestSolution);
      intersectionInfo.normal = planes[bestSolution.index].normal;
      intersectionInfo.color = planes[bestSolution.index].color;
      intersectionInfo.kA = planes[bestSolution.index].coeffs.x;
      intersectionInfo.kD = planes[bestSolution.index].coeffs.y;
      intersectionInfo.kS = planes[bestSolution.index].coeffs.z;
      intersectionInfo.type = ShapeType::kPlane;
      break;
    case ShapeType::kTriangle:
      intersectionInfo.point = ray.where(bestSolution.smallestSolution);
      intersectionInfo.normal = triangles[bestSolution.index].normal;
      intersectionInfo.color = triangles[bestSolution.index].color;
      intersectionInfo.kA = triangles[bestSolution.index].coeffs.x;
      intersectionInfo.kD = triangles[bestSolution.index].coeffs.y;
      intersectionInfo.kS = triangles[bestSolution.index].coeffs.z;
      intersectionInfo.type = ShapeType::kTriangle;
      break;
    case ShapeType::kLineSegment:
      intersectionInfo.point = ray.where(bestSolution.smallestSolution);
      intersectionInfo.normal = (intersectionInfo.point - lines[bestSolution.index].origin).getNormalized();
      intersectionInfo.color = lines[bestSolution.index].color;
      intersectionInfo.kA = 0.5;
      intersectionInfo.kD = 0.5;
      intersectionInfo.kS = 0.5;
      intersectionInfo.type = ShapeType::kLineSegment;
      break;
  default:
    break;
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
                const Vector<Line>& lines,
                int maxDepth = 3)
{
  Color colorResult(0, 0, 0);
  Color noIntersection(0, 0, 0);
  int depth = 0;
  Ray currentRay = ray;

  while (depth < maxDepth)
  {
    IntersectionInfo intersectionInfo = findClosestIntersection(currentRay, spheres, planes,triangles,lines);

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

float findTimeDelay(const Ray& ray,
                    const Vector<Sphere>& spheres,
                    const Vector<Plane>& planes,
                    const Vector<Triangle>& triangles)
{
  float timeDelay = 0.0f;
  Ray currentRay = ray;

  Vector<Line> lines;
  IntersectionInfo intersectionInfo = findClosestIntersection(currentRay, spheres, planes, triangles, lines);

  if (intersectionInfo.type != ShapeType::kEmpty)
  {

    timeDelay = ((intersectionInfo.point - currentRay.position).length() / SPEED_OF_SOUND) * 0.001f;

    Vector3 intersectionNormal = intersectionInfo.normal;
    currentRay.direction = currentRay.direction - 2 * (currentRay.direction.dot(intersectionNormal)) * intersectionNormal;
    currentRay.position = intersectionInfo.point;
  }
  
  return timeDelay;
}

int main()
{
  /*
  ComputeAPI computeAPI;

  computeAPI.Init();
  computeAPI.DecodeProgram("../compute/compute.cl");

  Complex<int> comp(3,4);
*/

//RandomEngine
  RandomEngine<REAL_TYPE> randomEngine;
  
  //Spheres
  Vector<Sphere> spheres;

  //spheres.emplace_back( Vector3(-36, -28, 220), 10, Vector3(139, 0, 0), kA, kD, kS);
  //spheres.emplace_back(Vector3(-55, -23, 230), 35, Vector3(255, 0, 0), kA, kD, kS);
  //spheres.emplace_back(Vector3(0, 0, 107), 10, Vector3(255, 69, 0), kA, kD, kS);
  //spheres.emplace_back(Vector3(13, -11, 235), 10, Vector3(255, 215, 0), kA, kD, kS);
  //spheres.emplace_back(Vector3(-151, -27, 220), 14, Vector3(128, 128, 0), kA, kD, kS);

  Vector3 soundPosition(-65, 25, 230) ;
  Vector3 soundDirection(0,0,1);
  spheres.emplace_back(soundPosition, 5, Vector3(255, 0, 255), kA, kD, kS);

  size_t soundRaysCount = 250;

  REAL_TYPE deg = ToDegrees(1.0f);
  REAL_TYPE rad = ToRadians(45.0);

  REAL_TYPE ratio = 0;
  for (size_t i = 0; i < soundRaysCount; ++i )
  { 

    Ray ray(soundPosition,soundDirection);

    ray.direction.Rotate(ToRadians(randomEngine.getRangedNumber(0,360)),AXIS::kX);
    ray.direction.Rotate(ToRadians(randomEngine.getRangedNumber(0,360)), AXIS::kY);
    ray.direction.Rotate(ToRadians(randomEngine.getRangedNumber(0,360)), AXIS::kZ);
    
    spheres.emplace_back(ray.where(60.0), 2, Vector3(255, 0, 0), kA, kD, kS);

    
  }

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

  Triangle triangle(Vector3(30, 23, 100), 
                    Vector3(20, 12, 100), 
                    Vector3(13, 26, 100), 
                    Vector3(0,255,255),kA,kD,kS);

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
  
  //scene.AddModelTriangles(attrib,shapes,kA,kD,kS);

  //Image creation
  Image image;
  image.create(vp.m_width, vp.m_height,32);

  

  //AUDIO HERE!
  Audio raytracedAudio;
  raytracedAudio.decode("../rsc/IR.wav");
  AudioBuffer raytracedAudioBuffer(raytracedAudio);

  /*############TESTING AREA##################*/

  Vector<Line> lines;
  lines.emplace_back(Vector3(0, 0, 0), Vector3(-36, -28, 220),Vector3(255,0,0));

  Vector<float> vTimeDelays;

  //Image processing
  for (int y = 0; y < vp.m_height; ++y)
  {
    for (int x = 0; x < vp.m_width; ++x)
    {
      Color pixelColor;
      float timeDelay = 0.0f;

      for (size_t j = 1; j <= AASamples; ++j)
      {
        float dx = (float)randomEngine.getNumber();
        float dy = (float)randomEngine.getNumber();
        
        Vector3 tmpPixel((-x + dx) / vp.m_ratio, (-y + dy) / vp.m_ratio, 0);
        Vector3 pixel = vp.m_upperLeftCorner + tmpPixel;

        Ray currentRay(scene.m_eye, pixel - scene.m_eye);
        pixelColor = pixelColor + findColor(currentRay, scene.m_spheres, scene.m_planes, scene.m_triangles, scene.m_light, lines, 1);
        
      }
      Color_BMP colorbmp;

      colorbmp.r = (unsigned char)std::min((REAL_TYPE)255, pixelColor.x / (REAL_TYPE)AASamples);
      colorbmp.g = (unsigned char)std::min((REAL_TYPE)255, pixelColor.y / (REAL_TYPE)AASamples);
      colorbmp.b = (unsigned char)std::min((REAL_TYPE)255, pixelColor.z / (REAL_TYPE)AASamples);
      colorbmp.a = 255;
      image.setPixel(x, y, colorbmp);
    }
  }


  
  //64 rayos - 
  //Basado en el cálculo tnego el 
  //Atenuación
  //Rayos tirados en múltiples direcciones. Montecarlo.
  
  /*DelayLine delayLine(raytracedAudioBuffer.m_samples.size());
  for(const auto& time : vTimeDelays)
  {
    
    delayLine.Process(raytracedAudioBuffer, time);
  }*/

  Audio outputAudioRaytraced;
  outputAudioRaytraced.create(raytracedAudioBuffer);
  outputAudioRaytraced.encode("../rsc/RAYTRACEDAUDIOFINAL.wav");
  image.encode(IMG_OUTPATH);

  return 0;
}
