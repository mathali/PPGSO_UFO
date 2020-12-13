#include "player.h"
#include "scene.h"
#include "cow.h"
#include "beam.h"
#include "explosion.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
  // Scale the default model
  scale *= 0.05f;

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("ufo.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ufo.obj");
}


bool Player::update(Scene &scene, float dt) {
  // Fire delay increment
  fireDelay += dt;

  // Hit detection
  for ( auto& obj : scene.objects ) {
    // Ignore self in scene
    if (obj.get() == this)
      continue;

    auto beam = dynamic_cast<Beam*>(obj.get());

    if (beam){
        beam->position = position - glm::vec3(0.0f, 6.2f, -4.3f); // + fireOffset
    }
  }

  // Keyboard controls
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

  // Firing beams
  if(scene.keyboard[GLFW_KEY_SPACE] && fireDelay > fireRate) {
    // Reset fire delay
    fireDelay = 0;
    // Invert file offset
    fireOffset = -fireOffset;
    if(!beam_flag) {
        auto beam = std::make_unique<Beam>();
        beam->position = position - glm::vec3(0.0f, 6.2f, -4.3f); // + fireOffset
        scene.objects.push_back(move(beam));
        beam_flag = true;
    }else{
        for ( auto& obj : scene.objects ) {
            if (obj.get() == this)
                continue;
            auto beam = dynamic_cast<Beam*>(obj.get());
            if (!beam) continue;
            beam->destroy();
        }
        beam_flag = false;
    }
  }

  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {
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
    shader->setUniform("object", 1);


    // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}
