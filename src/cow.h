#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class Cow final : public Object {
private:
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;

  float age{0.0f};

  bool m_jump = false;
  float height_offset = 0.0f;
  short int m_mode = glm::linearRand(0, 3);
  short direction = -1;
  float momentum = 20.0f;
  float m_timer = glm::linearRand(1.5f, 3.5f);
  glm::vec3 speed;

  // Material components defined for the object. Extracted from .mtl files downloaded with objects
  glm::vec3 Ka = {0.024155, 0.024155, 0.024155};
  glm::vec3 Kd = {1.963556, 1.963556, 1.963556};
  glm::vec3 Ks = {0.034077, 0.034077, 0.034077};
  int class_test = 3;


public:
  bool caught = false;
  float height_limit;
  bool keep = true;
  Cow();

  bool update(Scene &scene, float dt) override;

  void render(Scene &scene) override;

  void movementAnimation(Scene &scene, bool c_flag, float dt);

private:
};

