#include <glm/gtc/random.hpp>
#include "grass.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


std::unique_ptr<ppgso::Mesh> Grass::mesh;
std::unique_ptr<ppgso::Texture> Grass::texture;
std::unique_ptr<ppgso::Shader> Grass::shader;

// Represents the grass tufts in the first scene
// They do not interact with anything
Grass::Grass() {
    scale *=  0.002f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("nature/grass.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("nature/grass.obj");
}

bool Grass::update(Scene &scene, float dt) {
    generateModelMatrix();

    return true;
}

void Grass::render(Scene &scene) {
    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("Ambient", scene.ambient);
    shader->setUniform("SpecularStrength", scene.specularStrength);
    shader->setUniform("diffuse_strength", scene.diffuseStrength);
    shader->setUniform("ViewPosition", scene.camera->position);
    shader->setUniform("Ka", Ka);
    shader->setUniform("Kd", Kd);
    shader->setUniform("Ks", Ks);
    shader->setUniform("object", 6);

    shader->setUniform("BeamFlag", scene.beam_flag);
    shader->setUniform("BeamPos", scene.beam_pos - position);
    shader->setUniform("BeamDirection", scene.beam_pos - position);
    shader->setUniform("BeamColor", scene.beam_color);
    shader->setUniform("BeamCutoff", scene.beam_cutoff);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}


