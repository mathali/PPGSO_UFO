#include <glm/gtc/random.hpp>
#include "tree_shadow.h"

#include <shaders/transparent_vert_glsl.h>
#include <shaders/transparent_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Tree_shadow::mesh;
std::unique_ptr<ppgso::Texture> Tree_shadow::texture;
std::unique_ptr<ppgso::Shader> Tree_shadow::shader;

// Object that represents tree shadows in the first scene
Tree_shadow::Tree_shadow() {
    scale *=  2.0f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(transparent_vert_glsl, transparent_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("nature/tree.obj");
}

bool Tree_shadow::update(Scene &scene, float dt) {
    generateModelMatrix();

    return true;
}

void Tree_shadow::render(Scene &scene) {
    shader->use();

    shader->setUniform("vertexColor", {0, 0, 0});

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}


