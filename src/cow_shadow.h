#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "cow.h"

class Cow_shadow final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    std::shared_ptr<Cow> parent;

public:
    bool caught = false;

    Cow_shadow(std::shared_ptr<Cow> par);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    float age{0.0f};
private:
};
