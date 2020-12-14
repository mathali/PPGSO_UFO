#pragma once
#include <ppgso/ppgso.h>

#include "object.h"
#include "player.h"

class Beam final : public Object {
private:
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Texture> texture;

  float age{0.0f};
  glm::vec3 speed;
  Player *parent;
public:
  Beam(Player *par);

  bool update(Scene &scene, float dt) override;

  void render(Scene &scene) override;
};

