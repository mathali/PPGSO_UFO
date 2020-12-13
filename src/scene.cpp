#include "scene.h"
#include "player.h"
#include "ground.h"
#include "generator.h"
#include "house_shadow.h"
#include "tree_shadow.h"
#include "cow_shadow.h"
#include "beam.h"

void Scene::update(float time) {
  camera->update();
  lightDirection = {-2.5f+abs(sin(timer/40)*5), glm::clamp(abs(1+sin(timer/14)*2), 1.0, 3.0), 0};
  timer += time;


  // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(objects);
  while (i != std::end(objects)) {
    // Update and remove from list if needed
    auto player = dynamic_cast<Player*>(i->get());
    if(player){
        beam_flag = player->beam_flag;
        player_pos.x = player->position.x;
        player_pos.z = player->position.z;
        if(beam_flag) {
            beam_pos = {player_pos.x, player->position.y - 0.3f, player_pos.z - 4.3f};
            beam_direction = {0, -1, 0};
        }
    }

    auto house_shadow = dynamic_cast<House_shadow*>(i->get());
    if(house_shadow){
        house_shadow->scale.x = 0.006/lightDirection.y;
        house_shadow->position.x = house_shadow->original_position.x + (lightDirection.x / (-2.5f));
    }

    auto tree_shadow = dynamic_cast<Tree_shadow*>(i->get());
    if(tree_shadow){
        if(lightDirection.x > 0.5) {
            tree_shadow->scale.x = 0.0005;
            tree_shadow->rotation.y = -ppgso::PI/2;
            tree_shadow->scale.y = -2.0f / (lightDirection.y * lightDirection.y);
            tree_shadow->position.x = tree_shadow->original_position.x - 1.2f;
        }else if(lightDirection.x > -0.5){
            tree_shadow->scale.x = 1.5f;
            tree_shadow->scale.y = 0.03f;
            tree_shadow->rotation.y = 0;
        }
        else{
            tree_shadow->scale.x = 0.0005;
            tree_shadow->rotation.y = -ppgso::PI/2;
            tree_shadow->scale.y = 2.0f / (lightDirection.y * lightDirection.y);
            tree_shadow->position.x = tree_shadow->original_position.x - 0.5f;
        }
    }

    auto cow_shadow = dynamic_cast<Cow_shadow*>(i->get());
    if(cow_shadow){
        if(lightDirection.x > 0.5) {
            cow_shadow->scale.y = 0.1f / (lightDirection.y * lightDirection.y);
        }
    }

    auto ground = dynamic_cast<Ground*>(i->get());
    if(ground){
        if(ground->position.x - player_pos.x >= 90 ) {
            ground->position.x = ground->position.x - 180;
        }
        if(ground->position.x - player_pos.x <= -90 ) {
            ground->position.x = ground->position.x + 180;
        }
        if(ground->position.z - player_pos.z >= 100) {
            ground->position.z = ground->position.z - 200;
        }
        if(ground->position.z - player_pos.z <= -80) {
            ground->position.z = ground->position.z + 200;
        }

    }

    auto generator = dynamic_cast<Generator*>(i->get());

    auto obj = i->get();

    if(!ground &&  !generator && (abs(obj->position.x - player_pos.x) >= 80 || abs(obj->position.z - player_pos.z) >= 80)){ // Remove objects that are too distant
        objects.erase(i);
        ++i;
        continue;
    }

    if (!obj->update(*this, time))
      i = objects.erase(i);
    else
      ++i;
  }
}

void Scene::render() {
  // Simply render all objects
  Beam *beam = nullptr;
  auto i = std::begin(objects);
  while (i != std::end(objects)){
      auto beam_holder = dynamic_cast<Beam*>(i->get());
      if(beam_holder) {
          beam = beam_holder;
          i++;
          continue;
      }
      auto obj = i->get();
      obj->render(*this);
      i++;
  }
  if(beam){
      beam->render(*this);
  }
}

std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : objects) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  return intersected;
}
