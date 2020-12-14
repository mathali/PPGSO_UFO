#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class Grass final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Material components defined for the object. Extracted from .mtl files downloaded with objects
    glm::vec3 Ka = {0.024155, 0.024155, 0.024155};
    glm::vec3 Kd = {0.463556, 0.463556 ,0.463556};
    glm::vec3 Ks = {0.214077, 0.214077, 0.214077};

public:
    Grass();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;


private:
};

