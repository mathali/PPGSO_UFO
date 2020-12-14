#pragma once
#include <ppgso/ppgso.h>

#include "object.h"

class Explosion final : public Object {
private:
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Texture> texture;

  float age{0.0f};
  float maxAge{0.2f};
  glm::vec3 rotMomentum;
public:
  glm::vec3 speed;

  Explosion();

  bool update(Scene &scene, float dt) override;

  void render(Scene &scene) override;
};

