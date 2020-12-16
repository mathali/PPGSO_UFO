#pragma once

#include <ppgso/ppgso.h>

#include "object.h"

class Ground final : public Object {
private:
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;

  // Material components defined for the object. Extracted from .mtl files downloaded with objects
  glm::vec3 Ka = {0.01882, 0.01882, 0.01882};
  glm::vec3 Kd = {0.05882, 0.05882, 0.05882};
  glm::vec3 Ks = {0.0009, 0.0009, 0.0009};

public:
  Ground();

  bool update(Scene &scene, float dt) override;

  void render(Scene &scene) override;
};

