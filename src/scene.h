#pragma once
#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

class Scene {
  public:
    void update(float time);

    void render();

    std::unique_ptr<Camera> camera;

    std::list< std::shared_ptr<Object> > objects;

    std::map< int, int > keyboard;

    bool secondScene = false;

    glm::vec3 lightDirection{-3.0f, -9.0f, 0.0f};
    glm::vec3 ambient = {20.0f, 20.0f, 20.0f};
    glm::vec3 specularStrength = {10.0f, 10.0f, 10.0f};
    glm::vec3 diffuseStrength = {0.01f, 0.01f, 0.01f};

    float timer = 0.0f;
    bool beam_flag = false;
    bool scene1;
    int captured = 0;
    glm::vec3 beam_pos;
    glm::vec3 beam_color = {0, 1, 0};
    glm::vec3 beam_direction;
    float beam_cutoff = glm::cos(0.34906585);

    struct{
        int x, z;
    } player_pos;
};
