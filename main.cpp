#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"
using namespace std;

Vec3f cast_ray(Vec3f &dir, vector<Sphere> &spheres, vector<Light> &lights) {

  Vec3f point, N;

  float dif_light_intensity = 0;

  
  for (auto& sphere :spheres) {

    if (sphere.ray_intersect(dir)) {

      for (size_t i =0; i < lights.size(); i++) {

        point = dir*((sphere.centre * dir)/dir.norm());
        Vec3f light_dir = (lights[i].position - point).normalize();
        sphere.calculate_N(dir,N,point);
        dif_light_intensity += lights[i].intensity * std::max(0.f, light_dir*N);
      }
 
      return sphere.colour * dif_light_intensity;

    }

  }


  return Vec3f(0.4,0.4,0.3);
}



void render(std::vector<Sphere> &spheres, vector<Vec3f> &buffer, int const &width, int const &height, int const &fov, vector<Light> &lights) {

  for (size_t j = 0; j<height ; j++) {
    for (size_t i = 0; i<width; i++) {

      float x = (2* ( i + 0.5)/(float)width - 1) * tan(fov/2.)*width/(float)height;
      float y = -(2* ( j + 0.5)/(float)height - 1) * tan(fov/2.);

      Vec3f dir = Vec3f(x,y,-1).normalize();
      buffer[i+j*width] = cast_ray(dir,spheres,lights);

    }
  }

  std::ofstream ofs;
  ofs.open("./out.ppm");
  ofs << "P6\n" << width << " " << height << "\n255\n";
  for (size_t i = 0; i < height*width; i++ ) {
    for (size_t j =0; j<3;j++) {
      ofs << (char)(255*std::max(0.f,std::min(1.f,buffer[i][j])));
    }
  }

  ofs.close();

}

int main() {

  const int width = 1024;
  const int height = 768;
  const int fov = 1.7;

  std::vector<Vec3f> framebuffer(width*height);
  std::vector<Sphere> spheres;
  std::vector<Light> lights;


  Vec3f sphere_centre2(0.1,0.1,-1);
  Sphere sphere2(sphere_centre2,0.2,Vec3f(0.4,0.5,0.1));

  Vec3f sphere_centre3(-0.3,0.1,-2);
  Sphere sphere3(sphere_centre3,0.2,Vec3f(0.4,0.3,0.1));

  Vec3f sphere_centre1(0.3,0.3,-1);
  Sphere sphere1(sphere_centre1,0.2,Vec3f(0.1,0.5,0.1));


  lights.push_back(Light(Vec3f(2, 2,  -2), 1.5));

  spheres.push_back(sphere2);
  spheres.push_back(sphere1);
  spheres.push_back(sphere3);

  render(spheres,framebuffer,width,height,fov,lights);
  return 0;
}
