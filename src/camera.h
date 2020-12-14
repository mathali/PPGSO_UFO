#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

class Camera {
public:
  glm::vec3 up{0,1,0};
  glm::vec3 position{0,1,3};
  glm::vec3 back{0,1,-1};

  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;

  bool secondScene = false;

  // Keyframe struct used to animate the camera in the second scene
  struct keyFrame{
      glm::mat4 matrix;
      float t;
  } first, second, third, fourth, fifth, sixth, seventh;

  Camera(float fow = 75.0f, float ratio = 1.0f, float near = 0.1f, float far = 100.0f);

  void update();

  glm::vec3 cast(double u, double v);
};

