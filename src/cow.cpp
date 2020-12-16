#include <glm/gtc/random.hpp>
#include "cow.h"
#include "beam.h"
#include "grass.h"
#include "tree.h"
#include "house.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


std::unique_ptr<ppgso::Mesh> Cow::mesh;
std::unique_ptr<ppgso::Texture> Cow::texture;
std::unique_ptr<ppgso::Shader> Cow::shader;

Cow::Cow() {
  scale *=  0.2f;
  speed = {0.0f, -9.81f, 0.0f};
  rotation = {0, 0, glm::linearRand(-ppgso::PI, ppgso::PI)};

  if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Bull_texture.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("bull.obj");
}

bool Cow::update(Scene &scene, float dt) {
  if (position.y < height_limit)        // Jumps sometimes but cows below the limit, this fixes it
      position.y = height_limit;
  float temp_position = position.y;
  float org_x = position.x;
  float org_z = position.z;

  for (auto &obj : scene.objects) {
    if (obj.get() == this) continue;

    auto house = dynamic_cast<House*>(obj.get());

    // Explode a house if they are thrown at it by the player
    if(house && caught && (abs(position.x - house->position.x) <= 3 && abs(position.z - house->position.z) <= 3)){
        for (int i = 0; i <= glm::linearRand(4, 6); i++) {
            float x_offset = glm::linearRand(0.2f, 0.9f);
            float z_offset = glm::linearRand(0.2f, 0.9f);
            float y_offset = glm::linearRand(0.0f, 0.5f);
            glm::vec3 explosion_position = {house->position.x + x_offset, house->position.y + y_offset, house->position.z + z_offset};
            house->explode(scene, explosion_position);
        }
        keep = false;
    }

    if(!keep)return false;
    auto beam = dynamic_cast<Beam*>(obj.get());

    // Detect if the cow is caught in a beam and move it up towards the player
    if (beam && abs(position.x - beam->position.x) < 1.0f && abs(position.z - (beam->position.z-3.0)) < 1.2f){
      caught = true;

      position.x = beam->position.x;
      position.z = beam->position.z - 3.0;
      position.y += 0.1f;

      if(position.y > -4.5f) {
          scene.captured++;
          return false;
      }
    }
  }
  if(caught && position.y > -10.0f && temp_position == position.y){
      // If the beam is turned off simulate the momentum by having the cow fly in the previous movement direction
      if(-10.0f - position.y < -0.3f) {
          if(direction == 0){
              speed.x =  - momentum;
          }else if (direction == 1){
              speed.x = momentum;
          }else if (direction == 2){
              speed.z = -momentum;
          }else if (direction == 3){
              speed.z = momentum;
          }else if(direction == 4){
              speed.x = -momentum;
              speed.z = -momentum;
          }else if (direction == 5){
              speed.x = momentum;
              speed.z = momentum;
          }else if (direction == 6){
              speed.x = -momentum;
              speed.z = momentum;
          }else if (direction == 7){
              speed.x = momentum;
              speed.z = -momentum;
          }
          // accumulate gravitational acceleration and sum the speed
          speed.y -= 9.81f*dt;
          position += speed*dt;
      }
      // plant the cow on the ground if it's too close
      else {
          position.y = -10.0f;
          speed = {0.0f, -9.81f, 0.0f};
          caught = false;
      }
  }
  // Detect the direction the cow is moving
  if(caught) {
      if (position.x - org_x < 0 && position.z - org_z == 0) {
          direction = 0;
      } else if (position.x - org_x > 0 && position.z - org_z == 0) {
          direction = 1;
      } else if (position.z - org_z < 0 && position.x - org_x == 0) {
          direction = 2;
      } else if (position.z - org_z > 0 && position.x - org_x == 0) {
          direction = 3;
      } else if (position.x - org_x < 0 && position.z - org_z < 0) {
          direction = 4;
      } else if (position.x - org_x > 0 && position.z - org_z > 0) {
          direction = 5;
      } else if (position.x - org_x < 0 && position.z - org_z > 0) {
          direction = 6;
      } else if (position.x - org_x > 0 && position.z - org_z < 0) {
          direction = 7;
      } else {
          direction = -1;
      }
  }

  if(scene.scene1)movementAnimation(scene, caught, dt);
  generateModelMatrix();

  return true;
}


void Cow::render(Scene &scene) {
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
    shader->setUniform("object", class_test);

    shader->setUniform("BeamFlag", scene.beam_flag);
    shader->setUniform("BeamPos", scene.beam_pos - position);
    shader->setUniform("BeamDirection", scene.beam_pos - scene.beam_direction);
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

// Procedural animation that controls the movement of cows
void Cow::movementAnimation(Scene &scene, bool c_flag, float dt) {
    if (c_flag) return;  // Do not animate movement if the cow is caught
    m_timer -= dt;
    float org_x = position.x;
    float org_z = position.z;
    float org_rot = rotation.z;
    if (m_timer <= 0) {     // Check if the animation timer has run out
        m_timer = glm::linearRand(1.5f, 3.5f);
        if (m_mode == 2 && m_jump) {        // Make sure the cow doesn't get stuck in the air if it was jumping
            if (height_offset > 0.0f) position.y -= height_offset;
            height_offset = 0.0f;
            m_jump = false;
        }
        rotation.z = glm::linearRand(-ppgso::PI, ppgso::PI);
        m_mode = glm::linearRand(0, 3); // Chose a type of animation
    }
    if (m_mode == 0) return;
    float velocity = glm::linearRand(0.01f, 0.1f);

    // Move correctly according to the rotation of the cow
    position.x += velocity * sin(rotation.z);
    position.z -= velocity * -cos(rotation.z);
    // If in jumping animation, move cow up if it has not jumped, otherwise move it back down
    if (m_mode == 2 && !m_jump) {
        position.y += 3.0f * dt;
        height_offset += 3.0f * dt;
        if (height_offset >= 0.7f) m_jump = true;
    } else if (m_mode == 2 && m_jump) {
        position.y -= 3.0f * dt;
        height_offset -= 3.0f * dt;
        if (height_offset <= 0.2f) {
            position.y = height_limit;
            height_offset = 0.0f;
            m_jump = false;
        }
    }

    /**
    * Detects if they are getting close to another object
    * If they are, reset the animation and try to move away
     */
    for (auto &obj : scene.objects) {
        auto cow = dynamic_cast<Cow *>(obj.get());
        auto house = dynamic_cast<House *>(obj.get());
        auto tree = dynamic_cast<Tree *>(obj.get());
        if (cow && ((abs(position.x - cow->position.x) >= 0.5f && abs(position.x - cow->position.x) <= 2) &&
                    (abs(position.z - cow->position.z) >= 0.5f && abs(position.z - cow->position.z) <= 2))) {
            position.x = org_x;
            position.z = org_z;
            rotation.z = org_rot;
            m_timer = 0.0f;
            break;
        }

        if (tree && (abs(position.x - tree->position.x) <= 2 && abs(position.z - tree->position.z) <= 2)) {
            position.x = org_x;
            position.z = org_z;
            rotation.z = org_rot;
            m_timer = 0.0f;
            break;
        }

        if (house && (abs(position.x - house->position.x) <= 4 && abs(position.z - house->position.z) <= 4)) {
            position.x = org_x;
            position.z = org_z;
            rotation.z = org_rot;
            m_timer = 0.0f;
            break;
        }
    }
}

