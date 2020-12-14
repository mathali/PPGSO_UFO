#pragma once
#include <ppgso/ppgso.h>

#include "object.h"


class Player final : public Object {
private:
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;

  // Material components defined for the object. Extracted from .mtl files downloaded with objects
  glm::vec3 Ka = {0.058823, 0.058823, 0.058823};
  glm::vec3 Kd = {0.288235, 0.288235, 0.288235};
  glm::vec3 Ks = {0.000000, 0.000000, 0.000000};

  float fireDelay{0.0f};
  float fireRate{0.3f};

public:
  bool beam_flag = false;
  short int direction = -1;

  Player();

  bool update(Scene &scene, float dt) override;

  void render(Scene &scene) override;
};

