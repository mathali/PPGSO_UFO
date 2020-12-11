#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Simple asteroid object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 * It will move down along the Y axis and self delete when reaching below -10
 */
class Asteroid final : public Object {
private:
  // Static resources (Shared between instances)
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;
  //static std::unique_ptr<ppgso::Texture> texture_norm;

  // Age of the object in seconds
  float age{0.0f};

  bool caught = false;
  bool m_jump = false;
  float height_offset = 0.0f;
  short int m_mode = glm::linearRand(0, 3);
  short direction = -1;
  float momentum = 20.0f;
  float m_timer = glm::linearRand(1.5f, 3.5f);

    // Speed and rotational momentum
  glm::vec3 speed;
  glm::vec3 rotMomentum;


  glm::vec3 Ka = {0.024155, 0.024155, 0.024155};
    glm::vec3 Kd = {1.963556, 1.963556, 1.963556};
    glm::vec3 Ks = {0.034077, 0.034077, 0.034077};
    int class_test = 3;

  /*!
   * Split the asteroid into multiple pieces and spawn an explosion object.
   *
   * @param scene - Scene to place pieces and explosion into
   * @param explosionPosition - Initial position of the explosion
   * @param explosionScale - Scale of the explosion
   * @param pieces - Asteroid pieces to generate
   */
  void explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces);

public:
  float height_limit;
  bool keep = true;
  /*!
   * Create new asteroid
   */
  Asteroid();

  /*!
   * Update asteroid
   * @param scene Scene to interact with
   * @param dt Time delta for animation purposes
   * @return
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render asteroid
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;

  /*!
   * Custom click event for asteroid
   */
  void onClick(Scene &scene) override;

  void movementAnimation(Scene &scene, bool c_flag, float dt);

private:
};

