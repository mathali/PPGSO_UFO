#include <glm/gtc/random.hpp>
#include "cow_shadow.h"

#include <shaders/transparent_vert_glsl.h>
#include <shaders/transparent_frag_glsl.h>


std::unique_ptr<ppgso::Mesh> Cow_shadow::mesh;
std::unique_ptr<ppgso::Texture> Cow_shadow::texture;
std::unique_ptr<ppgso::Shader> Cow_shadow::shader;

Cow_shadow::Cow_shadow(std::shared_ptr<Cow> par) {
    scale *=  0.2f;
    scale.x = 0.02f;
    parent = move(par);  // Keep a reference to the parent cow
    position = parent->position;
    rotation = parent->rotation;
    rotation.y = ppgso::PI/2;

    if (!shader) shader = std::make_unique<ppgso::Shader>(transparent_vert_glsl, transparent_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Bull.obj");
}

bool Cow_shadow::update(Scene &scene, float dt) {
    if (!parent->keep || parent->position.y > -4.5f) return false;  // Destroy object if the parent has been removed
    caught = parent->caught;

    // Transform the shadow according to the light position
    if(scene.lightDirection.x < 0.5f && scene.lightDirection.x > -0.5f) {
        scale.x = 0.2f;
        scale.y = 0.02f;
        rotation.y = 0;
        rotation.z = parent->rotation.z;
    }
    else {
        scale.y = scene.lightDirection.x / 5;
        // Change scale based on the rotation of the cow
        if(abs(parent->rotation.z) > ppgso::PI/2){
            scale.z = 0.02f + (0.18f * ((abs(parent->rotation.z)-ppgso::PI/2)/(ppgso::PI/2)));
        }
        else{
            scale.z = 0.2f - (0.18f * (abs(parent->rotation.z)/(ppgso::PI/2)));
        }
        scale.x = 0.02f;
        rotation.y = ppgso::PI/2;
        rotation.z = 0;
    }
    position.x = parent->position.x;
    position.z = parent->position.z;
    generateModelMatrix();

    return true;
}

void Cow_shadow::render(Scene &scene) {
    if(caught)return;
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


