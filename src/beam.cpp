#include <glm/gtc/random.hpp>
#include "scene.h"
#include "beam.h"

#include <shaders/transparent_vert_glsl.h>
#include <shaders/transparent_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Beam::mesh;
std::unique_ptr<ppgso::Shader> Beam::shader;
std::unique_ptr<ppgso::Texture> Beam::texture;

// Object that represents the beam of the light emitting from the UFO
Beam::Beam(Player *par) {
  speed = {0.0f, -3.0f, 0.0f};
  scale.y *= 2.0f;
  parent = par;

  if (!shader) shader = std::make_unique<ppgso::Shader>(transparent_vert_glsl, transparent_frag_glsl);
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("beam.obj");
}

bool Beam::update(Scene &scene, float dt) {
  if (age > 5.0f ) return false;
  position = parent->position - glm::vec3(0.0f, 6.2f, -4.3f);
  generateModelMatrix();
  if(!parent->beam_flag) return false;
  else return true;
}

void Beam::render(Scene &scene) {

  shader->use();

  shader->setUniform("vertexColor", scene.beam_color);

  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  shader->setUniform("ModelMatrix", modelMatrix);

  mesh->render();
}
