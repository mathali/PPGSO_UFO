#include <glm/gtc/random.hpp>
#include "tree_shadow.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Tree_shadow::mesh;
std::unique_ptr<ppgso::Texture> Tree_shadow::texture;
std::unique_ptr<ppgso::Shader> Tree_shadow::shader;

Tree_shadow::Tree_shadow() {
    // Set random scale speed and rotation
    scale *=  2.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("nature/tree_test.obj");
}

bool Tree_shadow::update(Scene &scene, float dt) {
    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;
    generateModelMatrix();

    return true;
}

void Tree_shadow::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("vertexColor", {0, 0, 0});

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}


