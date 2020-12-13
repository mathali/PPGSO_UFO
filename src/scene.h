#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
  public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float time);

    /*!
     * Render all objects in the scene
     */
    void render();

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    //std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list< std::shared_ptr<Object> > objects;

    // Keyboard state
    std::map< int, int > keyboard;

    bool secondScene = false;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{-3.0f, -9.0f, 0.0f};
    glm::vec3 light_offset{5, 1.5f, 5};
    glm::vec3 ambient = {20.0f, 20.0f, 20.0f};
    glm::vec3 specularStrength = {10.0f, 10.0f, 10.0f};
    glm::vec3 diffuseStrength = {0.01f, 0.01f, 0.01f};
    float timer = 0.0f;
    float light_delay = 0.7f;
    bool beam_flag = false;
    bool scene1;
    int captured = 0;
    glm::vec3 beam_pos;
    glm::vec3 beam_color = {0, 1, 0};
    glm::vec3 beam_direction;
    float beam_cutoff = glm::cos(0.34906585);

    // Store cursor state
    struct {
      double x, y;
      bool left, right;
    } cursor;

    struct{
        int x, z;
    } player_pos;
};

#endif // _PPGSO_SCENE_H
