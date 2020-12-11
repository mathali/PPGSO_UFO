#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
  float fowInRad = (ppgso::PI/180.0f) * fow;

  projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
  first.matrix = {
          {0.7071,  0, -0.7071, 0},
          {0, 1, 0, 0},
          {0.7071,  0, 0.7071, 0},
          {14.5, -6, -15, 1}
  };
  first.t = 0.0f;

  second.matrix = {
          {1,  0, 0, 0},
          {0, 1, 0, 0},
          {0,  0, 1, 0},
          {14.5, -6, -8, 1}
  };
  second.t = 15.0f;

  third.matrix = {
          {1,  0, 0, 0},
          {0, 1, 0, 0},
          {0,  0, 1, 0},
          {0, -6, -8, 1}
  };
  third.t = 30.0f;

  fourth.matrix = {
          {1,  0, 0, 0},
          {0, 1, 0, 0},
          {0,  0, 1, 0},
          {-5, -6, -8, 1}
  };
  fourth.t = 45.0f;

  fifth.matrix =  {
          {1,  0,  0, 0},
          {0, 1, 0, 0},
          {0,  0, 1, 0},
          {-5, -6, 0, 1}
  };
  fifth.t = 60.0f;

  sixth.matrix = {
          {1,  0,  0, 0},
          {0, 1, 0, 0},
          {0,  0, 1, 0},
          {-5, -6, 8, 1}
  };
  sixth.t = 75.0f;

  seventh.matrix = {
          {0,  0,  -1, 0},
          {0, 1, 0, 0},
          {1,  0, 0, 0},
          {-15, -6, 0, 1}
  };
  seventh.t = 90.0f;
}

void Camera::update() {
  if(!secondScene) {
      viewMatrix = lookAt(position, position - back, up);
  }
}

glm::vec3 Camera::cast(double u, double v) {
  // Create point in Screen coordinates
  glm::vec4 screenPosition{u,v,0.0f,1.0f};

  // Use inverse matrices to get the point in world coordinates
  auto invProjection = glm::inverse(projectionMatrix);
  auto invView = glm::inverse(viewMatrix);

  // Compute position on the camera plane
  auto planePosition = invView * invProjection * screenPosition;
  planePosition /= planePosition.w;

  // Create direction vector
  auto direction = glm::normalize(planePosition - glm::vec4{position,1.0f});
  return glm::vec3{direction};
}
