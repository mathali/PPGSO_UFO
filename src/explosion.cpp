#include <glm/gtc/random.hpp>
#include "scene.h"
#include "explosion.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Explosion::mesh;
std::unique_ptr<ppgso::Texture> Explosion::texture;
std::unique_ptr<ppgso::Shader> Explosion::shader;

Explosion::Explosion() {
  rotation = glm::ballRand(ppgso::PI)*3.0f;
  rotMomentum = glm::ballRand(ppgso::PI)*3.0f;
  speed = {0.0f, 0.0f, 0.0f};
  scale *= glm::linearRand(1.0f, 4.0f);

  if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("explosion.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("explosion.obj");
}

void Explosion::render(Scene &scene) {
  shader->use();

  shader->setUniform("Transparency", 1.0f - age / maxAge);

  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);

  mesh->render();
}

bool Explosion::update(Scene &scene, float dt) {
  // from gl9
  scale = scale * ( 1.0f + dt * 5.0f);
  rotation += rotMomentum * dt;
  position += speed * dt;

  age += dt;
  if (age > maxAge) return false;

  generateModelMatrix();
  return true;
}
