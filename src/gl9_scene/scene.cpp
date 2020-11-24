#include "scene.h"
#include "player.h"
#include "space.h"
#include "generator.h"

void Scene::update(float time) {
  camera->update();
  lightDirection = {-2.5f+abs(sin(timer/40)*5), abs(1+sin(timer/50)*2), 0};
  timer += time;
  //std::cout << lightDirection.x << " " << lightDirection.y << std::endl;


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

    auto ground = dynamic_cast<Space*>(i->get());
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
      i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }
}

void Scene::render() {
  // Simply render all objects
  for ( auto& obj : objects )
    obj->render(*this);
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
