#include <glm/gtc/random.hpp>
#include "cow_shadow.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Cow_shadow::mesh;
std::unique_ptr<ppgso::Texture> Cow_shadow::texture;
std::unique_ptr<ppgso::Shader> Cow_shadow::shader;

Cow_shadow::Cow_shadow(std::shared_ptr<Asteroid> par) {
    // Set random scale speed and rotation
    scale *=  0.2f;
    scale.x = 0.02f;
    parent = move(par);
    position = parent->position;
    rotation = parent->rotation;
    rotation.y = ppgso::PI/2;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Bull.obj");
}

bool Cow_shadow::update(Scene &scene, float dt) {
    if (!parent->keep || parent->position.y > -4.5f) return false;
    if(scene.lightDirection.x < 0.5f && scene.lightDirection.x > -0.5f) {
        scale.x = 0.2f;
        scale.y = 0.02f;
        rotation.y = 0;
        rotation.z = parent->rotation.z;
    }
    else {
        scale.y = scene.lightDirection.x / 5;
        scale.x = 0.02f;
        rotation.y = ppgso::PI/2;
        rotation.z = 0;
    }
        //scale.y = -scale.y;
    position.x = parent->position.x;
    position.z = parent->position.z;
    //rotation.x = parent->rotation.x;
    //rotation.z = 0;//parent->rotation.z;
    generateModelMatrix();

    return true;
}

void Cow_shadow::render(Scene &scene) {
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


