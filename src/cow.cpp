#include <glm/gtc/random.hpp>
#include "cow.h"
#include "beam.h"
#include "grass.h"
#include "tree.h"
#include "house.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Cow::mesh;
std::unique_ptr<ppgso::Texture> Cow::texture;
std::unique_ptr<ppgso::Shader> Cow::shader;

Cow::Cow() {
  // Set random scale speed and rotation
  scale *=  0.2f;
  speed = {0.0f, -9.81f, 0.0f};
  rotation = {0, 0, glm::linearRand(-ppgso::PI, ppgso::PI)};
  rotMomentum = glm::ballRand(ppgso::PI);

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Bull_texture.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("bull.obj");
}

bool Cow::update(Scene &scene, float dt) {
  if (position.y < height_limit)
      position.y = height_limit;
  float temp_position = position.y;
  float org_x = position.x;
  float org_z = position.z;
  // Collide with scene
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and beams, ignore other objects
    auto house = dynamic_cast<House*>(obj.get()); // dynamic_pointer_cast<Cow>(obj);
    if(house && caught && (abs(position.x - house->position.x) <= 3 && abs(position.z - house->position.z) <= 3)){
        for (int i = 0; i <= glm::linearRand(4, 6); i++) {
            float x_offset = glm::linearRand(0.2f, 0.9f);
            float z_offset = glm::linearRand(0.2f, 0.9f);
            float y_offset = glm::linearRand(0.0f, 0.5f);
            glm::vec3 explosion_position = {house->position.x + x_offset, house->position.y + y_offset, house->position.z + z_offset};
            float explosion_scale = glm::linearRand(1.0f, 4.0f);
            house->explode(scene, explosion_position, {explosion_scale, explosion_scale, explosion_scale}, 3);
        }
        keep = false;
    }

    auto house_shadow = dynamic_cast<House_shadow*>(obj.get()); // dynamic_pointer_cast<Cow>(obj);
    if(house_shadow && caught && (abs(position.x - house_shadow->position.x) <= 4 && abs(position.z - house_shadow->position.z) <= 4)){
        house_shadow->age = 100.0f;
    }
    if(!keep)return false;
    auto beam = dynamic_cast<Beam*>(obj.get());

    // Compare distance to approximate size of the asteroid estimated from scale.
    if (beam && abs(position.x - beam->position.x) < 1.0f && abs(position.z - (beam->position.z-3.0)) < 1.2f){
      caught = true;

      position.x = beam->position.x;
      position.z = beam->position.z - 3.0;  //TODO: ORIGINAL -4
      position.y += 0.1f;

      // Destroy self
      if(position.y > -4.5f) {
          scene.captured++;
          return false;
      }
    }
  }
  if(caught && position.y > -10.0f && temp_position == position.y){
      if(-10.0f - position.y < -0.3f) {
          if(direction == 0){
              speed.x =  - momentum;
              //momentum -= 0.3f;
          }else if (direction == 1){
              speed.x = momentum;
              //momentum -= 0.3f;
          }else if (direction == 2){
              speed.z = -momentum;
              //momentum -= 0.3f;
          }else if (direction == 3){
              speed.z = momentum;
              //momentum -= 0.3f;
          }else if(direction == 4){
              speed.x = -momentum;
              speed.z = -momentum;
              //momentum -= 0.3f;
          }else if (direction == 5){
              speed.x = momentum;
              speed.z = momentum;
              //momentum -= 0.3f;
          }else if (direction == 6){
              speed.x = -momentum;
              speed.z = momentum;
              //momentum -= 0.3f;
          }else if (direction == 7){
              speed.x = momentum;
              speed.z = -momentum;
              //momentum -= 0.3f;
          }
          speed.y -= 9.81f*dt;
          position += speed*dt;
      }
      else {
          position.y = -10.0f;
          speed = {0.0f, -9.81f, 0.0f};
          caught = false;
      }
  }
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
  // Generate modelMatrix from position, rotation and scale
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
  //shader->setUniform("TextureNorm", *texture_norm);
  mesh->render();
}

void Cow::movementAnimation(Scene &scene, bool c_flag, float dt) {
    if (c_flag) return;
    m_timer -= dt;
    float org_x = position.x;
    float org_z = position.z;
    float org_rot = rotation.z;
    if (m_timer <= 0) {
        m_timer = glm::linearRand(1.5f, 3.5f);
        if (m_mode == 2 && m_jump) {
            if (height_offset > 0.0f) position.y -= height_offset;
            height_offset = 0.0f;
            m_jump = false;
        }
        rotation.z = glm::linearRand(-ppgso::PI, ppgso::PI);
        m_mode = glm::linearRand(0, 3);
    }
    if (m_mode == 0) return;
    float velocity = glm::linearRand(0.01f, 0.1f);

    position.x += velocity * sin(rotation.z);
    position.z -= velocity * -cos(rotation.z);
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

