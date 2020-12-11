#pragma once
#include <ppgso/ppgso.h>

#include "object.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Asteroid objects in the scene
 */
class Player final : public Object {
private:
  // Static resources (Shared between instances)
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 Ka = {0.058823, 0.058823, 0.058823};
    glm::vec3 Kd = {0.288235, 0.288235, 0.288235};
    glm::vec3 Ks = {0.000000, 0.000000, 0.000000};

  // Delay fire and fire rate
  float fireDelay{0.0f};
  float fireRate{0.1f};
  glm::vec3 fireOffset{0.7f,0.0f,0.0f};

public:
   bool beam_flag = false;
   short int direction = -1;
  /*!
   * Create a new player
   */
  Player();

  /*!
   * Update player position considering keyboard inputs
   * @param scene Scene to update
   * @param dt Time delta
   * @return true to delete the object
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render player
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;

};

