#include "Audio.h"
#include "Image.h"



#ifdef _WIN32
  #define INPATH "../rsc/Woosh.wav"
  #define OUTPATH "../rsc/out2.wav"
  #define OUTPATH2 "../rsc/out3.wav"

  #define IMG_INPATH "../rsc/TestImage.bmp"
  #define IMG_OUTPATH "../rsc/OutImage.bmp"
#endif

#ifdef __APPLE__
  #define INPATH "../../rsc/SoundFile_01.wav"
  #define OUTPATH "../../rsc/out2.wav"
  #define OUTPATH2 "../../rsc/out3.wav"

  #define IMG_INPATH "../../rsc/TestImage.bmp"
  #define IMG_OUTPATH "../../rsc/OutImage.bmp"
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
  /*audio.biquad(FilterType::LOWPASS,800.0,0.707f);
  audio.biquad(FilterType::HIGHPASS, 100.0, 0.707f);*/

  audio.encode(OUTPATH);

  Audio newAudio;
  uint32 milliseconds = 250;
  newAudio.create(96000, 16, 4, milliseconds);
  newAudio.sine(0.5f, 440.0f);
  newAudio.encode(OUTPATH2);

  /*
  Image image;
  image.decode(IMG_INPATH);

  image.encode(IMG_OUTPATH);

  Image image2;
  image2.create(256, 256, 32);
  image2.clearColor(Color(0, 0, 255, 255));
  image2.encode("../rsc/OutImage2.bmp");

  */
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
        const Vector3& eye = Vector3(0,0,0)) 
    : m_light(light),
      m_planes(planes),
      m_spheres(spheres),
      m_eye(eye)
  {}
  
 public:

  Vector3 m_eye;
  Light m_light;
  Vector<Plane> m_planes;
  Vector<Sphere> m_spheres;
  
};





IntersectionInfo findClosestIntersection(const Ray& ray,
                                        const Vector<Sphere>& spheres)
{
  IntersectionInfo intersectionInfo;

  int closestSphereIndex = 0;

  REAL_TYPE smallestSolutionSphere = 50000;

  size_t count = spheres.size();
  Vector<REAL_TYPE> solutions;
  Vector<size_t> index;

  for (size_t i = 0; i < count; ++i)
  {
    
    REAL_TYPE a = ray.direction.dot(ray.direction);
    REAL_TYPE b = 2 * (ray.direction.dot(ray.position - spheres[i].m_center));
    REAL_TYPE c = (ray.position - spheres[i].m_center).dot(ray.position - spheres[i].m_center) - spheres[i].m_radius * spheres[i].m_radius;

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

  if (smallestSolutionSphere < 50000)
  {
    intersectionInfo.point = ray.where(smallestSolutionSphere);
    intersectionInfo.normal = (intersectionInfo.point - spheres[closestSphereIndex].m_center).getNormalized();
    intersectionInfo.color = spheres[closestSphereIndex].m_color;
    intersectionInfo.kA = spheres[closestSphereIndex].m_coeffs.x;
    intersectionInfo.kD = spheres[closestSphereIndex].m_coeffs.y;
    intersectionInfo.kS = spheres[closestSphereIndex].m_coeffs.z;
    intersectionInfo.type = ShapeType::kSphere;
  }
  


  return intersectionInfo;
}

Color calculateColor(const IntersectionInfo& info, 
                     const Light& light, 
                     const Vector<Sphere>& spheres,
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
                const Light& light,
                int maxDepth = 3)
{
  Color colorResult(0, 0, 0);
  Color noIntersection(255, 255, 255);
  int depth = 0;
  Ray currentRay = ray;

  
  while (depth < maxDepth)
  {
    IntersectionInfo intersectionInfo = findClosestIntersection(currentRay, spheres);

    if (intersectionInfo.type != ShapeType::kEmpty)
    {
      depth = depth + 1;
      colorResult = colorResult + calculateColor(intersectionInfo, light, spheres, currentRay.direction);
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
  REAL_TYPE kS = (REAL_TYPE)0.3;

  //Spheres
  Vector<Sphere> spheres;

  spheres.emplace_back(Vector3(-36, -28, 220), 10, Vector3(139, 0, 0), kA, kD, kS);
  spheres.emplace_back(Vector3(-55, -23, 230), 35, Vector3(255, 0, 0), kA, kD, kS);
  spheres.emplace_back(Vector3(-27, -39, 157), 10, Vector3(255, 69, 0), kA, kD, kS);
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
  
  //Lights
  Vector3 eye(0, 0, 0);
  Vector3 lightPos(400, 400, 400);
  REAL_TYPE lightIntensity = 585000;
  REAL_TYPE ambientIntensity = 1;
  Light light(lightPos, lightIntensity, ambientIntensity);

  //Viewport
  Viewport vp(width, height);

  //Scene
  Scene scene(light,planes,spheres,eye);

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
        pixelColor = pixelColor + findColor(currentRay, scene.m_spheres, scene.m_planes, scene.m_light, 3);
      }
      Color_BMP colorbmp;

      colorbmp.r = (unsigned char)std::min((REAL_TYPE)255, pixelColor.x / (REAL_TYPE)AASamples);
      colorbmp.g = (unsigned char)std::min((REAL_TYPE)255, pixelColor.y / (REAL_TYPE)AASamples);
      colorbmp.b = (unsigned char)std::min((REAL_TYPE)255, pixelColor.z / (REAL_TYPE)AASamples);
      colorbmp.a = 255;
      image.setPixel(x, y, colorbmp);
    }
  }
  
  image.encode("raytracer.bmp");

  return 0;
}
