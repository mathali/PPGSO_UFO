#include <glm/gtc/random.hpp>
#include "asteroid.h"
#include "projectile.h"
#include "explosion.h"
#include "grass.h"
#include "tree1_1.h"
#include "house.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Asteroid::mesh;
std::unique_ptr<ppgso::Texture> Asteroid::texture;
std::unique_ptr<ppgso::Shader> Asteroid::shader;

Asteroid::Asteroid() {
  // Set random scale speed and rotation
  scale *=  0.2f;//glm::linearRand(1.0f, 3.0f);
  speed = {0.0f, -9.81f, 0.0f};
  rotation = {0, 0, glm::linearRand(-ppgso::PI, ppgso::PI)};
  rotMomentum = glm::ballRand(ppgso::PI);

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Bull_texture.bmp"));
  //if (!texture_norm) texture_norm = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("cow_norm.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("bull.obj");
}

bool Asteroid::update(Scene &scene, float dt) { //TODO: Figure out why cows sometimes disappear after landing
  // Count time alive
  //age += dt;

  // Animate position according to time
  //position += speed * dt;

  // Rotate the object
  //rotation += rotMomentum * dt;

  // Delete when alive longer than 10s or out of visibility
  if (position.y < height_limit)
      position.y = height_limit;
  float temp_position = position.y;
  float org_x = position.x;
  float org_z = position.z;
  // Collide with scene
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    auto house = dynamic_cast<House*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
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

    auto house_shadow = dynamic_cast<House_shadow*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
    if(house_shadow && caught && (abs(position.x - house_shadow->position.x) <= 4 && abs(position.z - house_shadow->position.z) <= 4)){
        house_shadow->age = 100.0f;
    }
    if(!keep)return false;
    auto projectile = dynamic_cast<Projectile*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
    //if (!projectile) continue;
    /*if(projectile) {
        std::cout << "Asteroid: " << position.x << " " << position.z << std::endl;
        std::cout << "Projectile: " << abs(position.x - projectile->position.x) << " " << abs(position.z - (projectile->position.z-4))<< std::endl;
    }*/

    // When colliding with other asteroids make sure the object is older than .5s
    // This prevents excessive collisions when asteroids explode.
    //if (asteroid && age < 0.5f) continue;

    /*auto player = dynamic_cast<Player*>(obj.get());
    if(player && (abs(position.x - player->position.x) >= 80 || abs(position.z - player->position.z >= 80))){
        return false;
    }*/

    // Compare distance to approximate size of the asteroid estimated from scale.
    if (projectile && abs(position.x - projectile->position.x) < 1.0f && abs(position.z - (projectile->position.z-3.0)) < 1.2f){ //distance(position, obj->position) < (obj->scale.z + scale.z) * 0.7f) {
      int pieces = 3;                                                                                                  //TODO: ORIGINAL -4
      caught = true;
      // Too small to split into pieces
      if (scale.y < 0.5) pieces = 0;

      position.x = projectile->position.x;
      position.z = projectile->position.z - 3.0;  //TODO: ORIGINAL -4
      position.y += 0.1f;

      // The projectile will be destroyed
      //if (projectile) projectile->destroy();

      // Generate smaller asteroids
      //explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

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

void Asteroid::explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces) {
  // Generate explosion
  auto explosion = std::make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed = speed / 2.0f;
  scene.objects.push_back(move(explosion));

  // Generate smaller asteroids
  for (int i = 0; i < pieces; i++) {
    auto asteroid = std::make_unique<Asteroid>();
    asteroid->speed = speed + glm::vec3(glm::linearRand(-3.0f, 3.0f), glm::linearRand(0.0f, -5.0f), 0.0f);;
    asteroid->position = position;
    asteroid->rotMomentum = rotMomentum;
    float factor = (float) pieces / 2.0f;
    asteroid->scale = scale / factor;
    scene.objects.push_back(move(asteroid));
  }
}

void Asteroid::render(Scene &scene) {
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

void Asteroid::onClick(Scene &scene) {
  std::cout << "Asteroid clicked!" << std::endl;
  explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
  age = 10000;
}


void Asteroid::movementAnimation(Scene &scene, bool c_flag, float dt) {
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
        auto cow = dynamic_cast<Asteroid *>(obj.get());
        auto house = dynamic_cast<House *>(obj.get());
        auto tree = dynamic_cast<Tree1_1 *>(obj.get());
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

