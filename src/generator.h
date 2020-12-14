#pragma once
#include "object.h"
#include "scene.h"


class Generator final : public Object {
public:
  bool update(Scene &scene, float dt) override;

  void render(Scene &scene) override;

  float time = 0.0f;
  bool flag = true;
};
