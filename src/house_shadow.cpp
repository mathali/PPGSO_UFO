#include <glm/gtc/random.hpp>
#include "house_shadow.h"

#include <shaders/transparent_vert_glsl.h>
#include <shaders/transparent_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> House_shadow::mesh;
std::unique_ptr<ppgso::Texture> House_shadow::texture;
std::unique_ptr<ppgso::Shader> House_shadow::shader;

House_shadow::House_shadow() {
    // Set random scale speed and rotation
    scale *=  0.004f;//glm::linearRand(1.0f, 3.0f);
    rotation = {0, 0, glm::linearRand(-3.0f, 3.0f)};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(transparent_vert_glsl, transparent_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("house.obj");
}

bool House_shadow::update(Scene &scene, float dt) {
    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;
    generateModelMatrix();

    return true;
}

void House_shadow::render(Scene &scene) {
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


