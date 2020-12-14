#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "house.h"

class House_shadow final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    std::shared_ptr<House> parent;

public:
    glm::vec3 original_position;

    House_shadow(std::shared_ptr<House> par);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

private:
};

