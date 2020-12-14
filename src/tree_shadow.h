#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class Tree_shadow final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;


public:
    glm::vec3 original_position;

    Tree_shadow();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
private:
};
