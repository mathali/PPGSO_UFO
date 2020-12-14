#include "ground.h"
#include "scene.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


// Squares that represent the ground of the first scene
Ground::Ground() {
  position = {0, -12, 0};
  rotation = {-ppgso::PI/2, 0, 0};
  scale = {20, 20, 1};
  if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("nature/ground.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Ground::update(Scene &scene, float dt) {
  generateModelMatrix();
  return true;
}

void Ground::render(Scene &scene) {

    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("Ambient", scene.ambient);
    shader->setUniform("SpecularStrength", scene.specularStrength);
    shader->setUniform("diffuse_strength", scene.diffuseStrength);
    shader->setUniform("ViewPosition", scene.camera->position);
    shader->setUniform("Ka", Ka);
    shader->setUniform("Kd", Kd);
    shader->setUniform("Ks", Ks);
    shader->setUniform("object", 5);

    shader->setUniform("BeamFlag", scene.beam_flag);
    shader->setUniform("BeamPos", scene.beam_pos);
    shader->setUniform("BeamDirection", scene.beam_direction);
    shader->setUniform("BeamColor", scene.beam_color);
    shader->setUniform("BeamCutoff", scene.beam_cutoff);


    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();

}

std::unique_ptr<ppgso::Mesh> Ground::mesh;
std::unique_ptr<ppgso::Shader> Ground::shader;
std::unique_ptr<ppgso::Texture> Ground::texture;
