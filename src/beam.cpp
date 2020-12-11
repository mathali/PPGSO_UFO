#include <glm/gtc/random.hpp>
#include "scene.h"
#include "beam.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>


// shared resources
std::unique_ptr<ppgso::Mesh> Beam::mesh;
std::unique_ptr<ppgso::Shader> Beam::shader;
std::unique_ptr<ppgso::Texture> Beam::texture;

Beam::Beam() {
  // Set default speed
  speed = {0.0f, -3.0f, 0.0f};
  scale.y *= 2.0f;

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("missile.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("missile.obj");
}

bool Beam::update(Scene &scene, float dt) {
  // Die after 5s
  if (age > 5.0f ) return false;//|| position.y < -10.0f) return false;
  generateModelMatrix();
  return true;
}

void Beam::render(Scene &scene) {

  shader->use();

  shader->setUniform("vertexColor", scene.beam_color);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);

  mesh->render();
}

void Beam::destroy() {
  // This will destroy the projectile on Update
  age = 100.0f;
}
