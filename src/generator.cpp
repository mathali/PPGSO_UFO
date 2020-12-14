#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "generator.h"
#include "cow.h"
#include "house.h"
#include "tree.h"
#include "grass.h"
#include "player.h"
#include "house_shadow.h"
#include "tree_shadow.h"
#include "cow_shadow.h"

// Object responsible for procedurally generating most other objects in the scene
bool Generator::update(Scene &scene, float dt) {

  unsigned int no_objects = scene.objects.size();
  if(!flag && no_objects < 500) { // Limit the number of objects that can be generated
                        // Prevents overlapping generation and reduces resource strain by not working with objects that the player cannot see
      for (auto &obj : scene.objects) {
          if (obj.get() == this) continue;
          auto player = dynamic_cast<Player *>(obj.get());
          // Move the generator position if the player is too far from the original position
          if (player && (abs(position.x - player->position.x) >= 40 || abs(position.z - player->position.z) >= 40)) {
              // Choose where to generate a new patch of objects based on the direction the player is heading
              switch (player->direction){
                  case 0:
                      position.x = player->position.x+40;
                      break;
                  case 1:
                      position.x = player->position.x-40;
                      break;
                  case 2:
                      position.z = player->position.z+40;
                      break;
                  case 3:
                      position.z = player->position.z-40;
                      break;
                  case 4:
                      position.x = player->position.x+40;
                      position.z = player->position.z+40;
                      break;
                  case 5:
                      position.x = player->position.x-40;
                      position.z = player->position.z+40;
                      break;
                  case 6:
                      position.x = player->position.x+40;
                      position.z = player->position.z-40;
                      break;
                  case 7:
                      position.x = player->position.x-40;
                      position.z = player->position.z-40;
                      break;
                  default:
                      break;
              }
              flag = true;
          }
          if(player){
              break;
          }
      }
  }

  time += dt;
  float pos_x = position.x - 40;
  float pos_z = position.z - 40;

  // Generate objects on a fresh patch of land
  if (flag) {
      bool occupied[100][100];          // Keep a checkboard pattern to remember which parts are occupied
      // Offsets specify how much space a given object takes up
      short unsigned int offset_cow = 2;
      short unsigned int offset_house = 6;
      short unsigned int offset_grass = 0.5;

      // Generate 25 cows on random unoccupied positions
      for( int x = 0; x < 25; x++) {
          int rand_x, rand_z;

          // Generate coordinates until you find an empty space
          do {
              rand_x = int(pos_x) + glm::linearRand(0, 80);
              rand_z = int(pos_z) + glm::linearRand(0, 80);
          }while(occupied[rand_x - int(pos_x)][rand_z - int(pos_z)]);

          // Mark the surrounding squares as occupied
          for(int i = (rand_x - int(pos_x))-offset_cow; i < (rand_x - int(pos_x))+offset_cow; i++){
              for(int j = (rand_z - int(pos_z))-offset_cow; j < (rand_z - int(pos_z))+offset_cow; j++){
                  occupied[i][j] = true;
              }
          }
          auto obj = std::make_shared<Cow>();
          obj->position = position;
          obj->height_limit = position.y;
          obj->position.x = rand_x;
          obj->position.z = rand_z;
          auto cow_shadow = std::make_unique<Cow_shadow>(obj);
          scene.objects.push_back(move(obj));
          scene.objects.push_back(move(cow_shadow));
      }

      // Generate a random amount of houses
      for( int x = 0; x < glm::linearRand(8, 12); x++) {

          int rand_x, rand_z;

          // Generate coordinates until you find an empty space
          do {
              rand_x = int(pos_x) + glm::linearRand(0, 80);
              rand_z = int(pos_z) + glm::linearRand(0, 80);
          }while(occupied[rand_x - int(pos_x)][rand_z - int(pos_z)]);

          // Mark surrounding squares as occupied
          for(int i = (rand_x - int(pos_x))-offset_house; i < (rand_x - int(pos_x))+offset_house; i++){
              for(int j = (rand_z - int(pos_z))-offset_house; j < (rand_z - int(pos_z))+offset_house; j++){
                  occupied[i][j] = true;
              }
          }
          auto obj = std::make_shared<House>();
          obj->position = position;
          obj->position.x = rand_x;
          obj->position.z = rand_z;
          auto house_shadow = std::make_unique<House_shadow>(obj);
          house_shadow->position = obj->position;
          house_shadow->original_position = obj->position;
          house_shadow->position.x += 1;
          house_shadow->rotation.z = obj->rotation.z;
          house_shadow->scale.y = 0.0005;
          scene.objects.push_back(move(obj));
          scene.objects.push_back(move(house_shadow));
      }

      // Generate a random amount of trees
      for( int x = 0; x < glm::linearRand(8, 12); x++) {

          int rand_x, rand_z;

          // Generate coordinates until you find an empty space
          do {
              rand_x = int(pos_x) + glm::linearRand(0, 80);
              rand_z = int(pos_z) + glm::linearRand(0, 80);
          }while(occupied[rand_x - int(pos_x)][rand_z - int(pos_z)]);

          // Mark surrounding squares as occupied
          for(int i = (rand_x - int(pos_x))-offset_cow; i < (rand_x - int(pos_x))+offset_cow; i++){
              for(int j = (rand_z - int(pos_z))-offset_cow; j < (rand_z - int(pos_z))+offset_cow; j++){
                  occupied[i][j] = true;
              }
          }

          auto obj1 = std::make_unique<Tree>();
          obj1->position.y = position.y;
          obj1->position.x = rand_x;
          obj1->position.z = rand_z;

          auto tree_shadow = std::make_unique<Tree_shadow>();
          tree_shadow->position.x = obj1->position.x-0.1f;
          tree_shadow->position.y = obj1->position.y;
          tree_shadow->position.z = obj1->position.z;
          tree_shadow->original_position = obj1->position;

          tree_shadow->rotation.y = -ppgso::PI/2;
          tree_shadow->scale.x = 0.0005;
          tree_shadow->scale.y = -tree_shadow->scale.y;
          scene.objects.push_back(move(obj1));
          scene.objects.push_back(move(tree_shadow));
      }

      // Generate a random amount of grass tufts
      for( int x = 0; x < glm::linearRand(80, 120); x++) {

          int rand_x, rand_z;

          // Generate coordinates until you find an empty space
          do {
              rand_x = int(pos_x) + glm::linearRand(0, 80);
              rand_z = int(pos_z) + glm::linearRand(0, 80);
          }while(occupied[rand_x - int(pos_x)][rand_z - int(pos_z)]);

          // Mark surrounding squares as occupied
          for(int i = (rand_x - int(pos_x))-offset_grass; i < (rand_x - int(pos_x))+offset_grass; i++){
              for(int j = (rand_z - int(pos_z))-offset_grass; j < (rand_z - int(pos_z))+offset_grass; j++){
                  occupied[i][j] = true;
              }
          }
          auto obj = std::make_unique<Grass>();
          obj->position.y = position.y + 0.1f;
          obj->position.x = rand_x;
          obj->position.z = rand_z;
          scene.objects.push_back(move(obj));
      }
    time = 0;
    flag = false;
  }

  return true;
}

void Generator::render(Scene &scene) {}
