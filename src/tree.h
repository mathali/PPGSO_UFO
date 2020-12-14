#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Simple house object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 */
class Tree final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Material components defined for the object. Extracted from .mtl files downloaded with objects
    glm::vec3 Ka = {0.036861, 0.036861, 0.036861};
    glm::vec3 Kd = {3.640506, 3.640506, 3.640506};
    glm::vec3 Ks = {0.210769, 0.210769, 0.210769};

public:
    Tree();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

private:
};

