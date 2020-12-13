#include <glm/gtc/random.hpp>
#include "tree.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Tree::mesh;
std::unique_ptr<ppgso::Texture> Tree::texture;
std::unique_ptr<ppgso::Shader> Tree::shader;

Tree::Tree() {
    scale *=  2.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("nature/tree.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("nature/tree.obj");
}

bool Tree::update(Scene &scene, float dt) {
    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Tree::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("Ambient", scene.ambient);
    shader->setUniform("SpecularStrength", scene.specularStrength);
    shader->setUniform("diffuse_strength", scene.diffuseStrength);
    shader->setUniform("ViewPosition", scene.camera->position);
    shader->setUniform("Ka", Ka);
    shader->setUniform("Kd", Kd);
    shader->setUniform("Ks", Ks);
    shader->setUniform("object", 2);

    shader->setUniform("BeamFlag", scene.beam_flag);
    shader->setUniform("BeamPos", scene.beam_pos - position);
    shader->setUniform("BeamDirection", scene.beam_pos - position);
    shader->setUniform("BeamColor", scene.beam_color);
    shader->setUniform("BeamCutoff", scene.beam_cutoff);


    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
