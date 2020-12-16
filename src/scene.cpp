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
  // Move lightDirection according to elapsed time to simulate the movement of the sun
  lightDirection = {-2.5f+abs(sin(timer/40)*5), glm::clamp(abs(1+sin(timer/14)*2), 1.0, 3.0), 0};
  timer += time;


  auto i = std::begin(objects);
  while (i != std::end(objects)) {
    auto player = dynamic_cast<Player*>(i->get());
    if(player){
        // move beam along with the player object if it is activated
        beam_flag = player->beam_flag;
        player_pos.x = player->position.x;
        player_pos.z = player->position.z;
        if(beam_flag) {
            beam_pos = {player_pos.x, player->position.y - 0.3f, player_pos.z - 4.3f};
            beam_direction = {0, -1, 0};
        }
    }

    /**
     * The following section specifies the movement and transformation of shadows in accordance with the movement
     * of the 'Sun'
     */

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

    /**
    * Move the generated ground squares according to the position of the player to create an illusion of a continuos
    * surface
    */
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

    // Remove objects that are too distant
    if(!ground &&  !generator && (abs(obj->position.x - player_pos.x) >= 80 || abs(obj->position.z - player_pos.z) >= 80)){
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
  Beam *beam = nullptr;
  auto i = std::begin(objects);
  while (i != std::end(objects)){
      // modified rendering structure to make sure the beam always renders last, otherwise it's transparency breaks
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