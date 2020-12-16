#include "player.h"
#include "scene.h"
#include "cow.h"
#include "beam.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

// Renders a UFO object that represents the player
Player::Player() {
  scale *= 0.05f;

  if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("ufo.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ufo.obj");
}


bool Player::update(Scene &scene, float dt) {
  fireDelay += dt;

  /**
  * Handle the movement of the player model
  * Also update the camera to stay in the same relative position
  */
  if(scene.keyboard[GLFW_KEY_A]) {
    direction = 0;
    position.x += 15 * dt;
    scene.camera->position.x = position.x;
    rotation.z = -ppgso::PI/4.0f;
  } else if(scene.keyboard[GLFW_KEY_D]) {
    direction = 1;
    position.x -= 15 * dt;
    scene.camera->position.x = position.x;
    rotation.z = ppgso::PI/4.0f;
  }else{
    direction = -1;
    rotation.z = 0;
  }
  if(scene.keyboard[GLFW_KEY_W]) {
    if(direction == -1)direction = 2;
    else direction = 4 + direction;
    position.z += 15 * dt;
    scene.camera->position.z += 15 * dt;
    rotation.x = ppgso::PI/30.0f;
  } else if(scene.keyboard[GLFW_KEY_S]) {
    if(direction == -1)direction = 3;
    else direction = 6 + direction;
    position.z -= 15 * dt;
    scene.camera->position.z -= 15 * dt;
    rotation.x = -ppgso::PI/20.0f;
  } else {
    rotation.x = 0;
  }

  // Turn on/off the beam of light
  if(scene.keyboard[GLFW_KEY_SPACE] && fireDelay > fireRate) {
    fireDelay = 0;
    if(!beam_flag) {
        auto beam = std::make_unique<Beam>(this);
        beam->position = position - glm::vec3(0.0f, 6.2f, -4.3f);
        scene.objects.push_back(move(beam));
        beam_flag = true;
    }else{
        beam_flag = false;
    }
  }

  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {
  shader->use();

  shader->setUniform("LightDirection", scene.lightDirection);
  shader->setUniform("Ambient", scene.ambient);
  shader->setUniform("SpecularStrength", scene.specularStrength);
  shader->setUniform("diffuse_strength", scene.diffuseStrength);
  shader->setUniform("ViewPosition", scene.camera->position);
  shader->setUniform("Ka", Ka);
  shader->setUniform("Kd", Kd);
  shader->setUniform("Ks", Ks);
  shader->setUniform("object", 1);

  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

