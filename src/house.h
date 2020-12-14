#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class House final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 Ka = {0.024155, 0.024155, 0.024155};
    glm::vec3 Kd = {0.737255, 0.737255, 0.737255};
    glm::vec3 Ks = {0.034077, 0.034077, 0.034077};



public:
    House();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void explode(Scene &scene, glm::vec3 explosionPosition);

float age{0.0f};
private:
};

