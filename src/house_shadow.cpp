#include <glm/gtc/random.hpp>
#include "house_shadow.h"

#include <shaders/transparent_vert_glsl.h>
#include <shaders/transparent_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> House_shadow::mesh;
std::unique_ptr<ppgso::Texture> House_shadow::texture;
std::unique_ptr<ppgso::Shader> House_shadow::shader;

// Represents the shadows of houses
House_shadow::House_shadow(std::shared_ptr<House> par) {
    parent = move(par); // Bind the parent so we can remove the shadow if the parent doesn't exist
    scale *=  0.004f;
    rotation = {0, 0, glm::linearRand(-3.0f, 3.0f)};

    if (!shader) shader = std::make_unique<ppgso::Shader>(transparent_vert_glsl, transparent_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("house.obj");
}

bool House_shadow::update(Scene &scene, float dt) {
    if(parent->age > 0.0f) return false;  // CHeck if the parent still exists
    generateModelMatrix();

    return true;
}

void House_shadow::render(Scene &scene) {
    shader->use();

    shader->setUniform("vertexColor", {0, 0, 0});

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}


