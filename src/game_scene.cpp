// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Cow and Ground objects
// - Contains a generator object that does not render but adds Cows to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>
#include <string>
#include <glm/gtx/matrix_interpolation.hpp>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "player.h"
#include "ground.h"
#include "cow.h"
#include "room_wall.h"
#include "cage.h"

const unsigned int SIZE = 512;

int count = 0;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
  Scene scene;
  bool animate = true;

  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene() {
    scene.scene1 = true;
    scene.secondScene = false;
    scene.captured = 0;
    scene.objects.clear();
    scene.timer = 0.0f;

    // Create a camera
    auto camera = std::make_unique<Camera>(75.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -15.0f;
    camera->position.y = 7.0f;
    camera->secondScene = false;
    scene.camera = move(camera);

    // Add ground background

    float ground_pos;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            auto ground = std::make_unique<Ground>();
            ground->position = {j*40 - 80, -10.8, i*40 - 40};
            ground_pos = ground->position.y;
            scene.objects.push_back(move(ground));
        }
    }


    // Add generator to scene
    auto generator = std::make_unique<Generator>();
    generator->position = {0, ground_pos + 0.8f, 0};
    scene.objects.push_back(move(generator));

    // Add player to the scene
    auto player = std::make_unique<Player>();
    player->position.y = -3;  //TODO: ORIGINAL -5
    scene.objects.push_back(move(player));
  }

  void initSecondScene(){
      scene.scene1 = false;
      scene.objects.clear();
      scene.player_pos.x = 0;
      scene.player_pos.z = 0;
      auto camera = std::make_unique<Camera>(75.0f, 1.0f, 0.1f, 100.0f);
      camera->position.z = -20.0f;
      camera->position.x = 0.0f;
      camera->position.y = 5.0f;
      camera->secondScene = true;
      camera->back = {camera->position.x, 0, camera->position.z};
      scene.camera = move(camera);

      for(int j = 0; j < 4; j++)
          for(int i = 0; i < 7; i++) {
              auto cage = std::make_unique<Cage>();
              cage->position = {15 + -5*i, 1, j*10 - 15};
              scene.objects.push_back(move(cage));

              if(scene.captured > 0) {
                  auto cow = std::make_unique<Cow>();
                  cow->position = {15 + -5 * i + 2.2, 3.5, j * 10 - 15 - 2.2};
                  cow->scale = {0.5, 0.5, 0.5};
                  cow->rotation = {0, 0, 0};
                  scene.objects.push_back(move(cow));
                  scene.captured--;
              }
          }
      auto floor = std::make_unique<Room_wall>();
      floor->position = {0, 0, 0};
      scene.objects.push_back(move(floor));

      auto ceiling = std::make_unique<Room_wall>();
      ceiling->position = {0, 20, 0};
      ceiling->rotation = {-3*ppgso::PI/2, 0, 0};
      scene.objects.push_back(move(ceiling));

      auto side1 = std::make_unique<Room_wall>();
      side1->position = {0, 10, 20};
      side1->rotation = {-ppgso::PI, 0, 0};
      scene.objects.push_back(move(side1));

      auto side2 = std::make_unique<Room_wall>();
      side2->position = {20, 10, 0};
      side2->rotation = {-ppgso::PI, ppgso::PI, -3*ppgso::PI/2};
      scene.objects.push_back(move(side2));

      auto side3 = std::make_unique<Room_wall>();
      side3->position = {-20, 10, 0};
      side3->rotation = {-ppgso::PI, ppgso::PI, 3*ppgso::PI/2};
      scene.objects.push_back(move(side3));

      auto side4 = std::make_unique<Room_wall>();
      side4->position = {0, 10, -20};
      side4->rotation = {-ppgso::PI, 0, ppgso::PI};
      scene.objects.push_back(move(side4));




  }
public:
  /*!
   * Construct custom game window
   */
  SceneWindow() : Window{"gl9_scene", 1280, 720} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initScene();
  }

  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        scene.beam_color = {0, 1, 0};
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        scene.beam_color = {1, 0, 0};
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        scene.beam_color = {0, 0, 1};
    }
    if (key == GLFW_KEY_UP) {
        scene.camera->position.z += 0.1f;
        scene.camera->position.y -= 0.1f;
    }

    if (key == GLFW_KEY_DOWN) {
        scene.camera->position.z -= 0.1f;
        scene.camera->position.y += 0.1f;
    }

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }
  }

    glm::mat4 bezierPoint(const glm::mat4 CP0,const glm::mat4 CP1,const glm::mat4 CP2 , float t) {
        glm::mat4 out;
        glm::mat4 supp1_1 = glm::interpolate(CP0, CP1, t);
        glm::mat4 supp1_2 = glm::interpolate(CP1, CP2, t);
        out = glm::interpolate(supp1_1, supp1_2, t);

        return out;
    }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Update and render all objects
    if(scene.timer < 30.0f || scene.secondScene) {
        scene.update(dt);
        scene.render();
        if(scene.secondScene){
            if(scene.timer < scene.camera->second.t) {
                scene.camera->viewMatrix = bezierPoint(scene.camera->first.matrix, scene.camera->second.matrix, scene.camera->third.matrix,
                                                       scene.timer / (scene.camera->second.t - scene.camera->first.t));
            }else if(scene.timer < scene.camera->third.t) {
                scene.camera->viewMatrix = bezierPoint(scene.camera->second.matrix, scene.camera->third.matrix, scene.camera->fourth.matrix,
                                                       (scene.timer - scene.camera->second.t) / (scene.camera->third.t - scene.camera->second.t));
            }else if(scene.timer < scene.camera->fourth.t) {
                scene.camera->viewMatrix = bezierPoint(scene.camera->third.matrix, scene.camera->fourth.matrix, scene.camera->fifth.matrix,
                                                       (scene.timer-scene.camera->third.t) / (scene.camera->fourth.t - scene.camera->third.t));
            }else if(scene.timer < scene.camera->fifth.t) {
                scene.camera->viewMatrix = bezierPoint(scene.camera->fourth.matrix, scene.camera->fifth.matrix, scene.camera->sixth.matrix,
                                                       (scene.timer-scene.camera->fourth.t) / (scene.camera->fifth.t - scene.camera->fourth.t));
            }else if(scene.timer < scene.camera->sixth.t) {
                scene.camera->viewMatrix = bezierPoint(scene.camera->fifth.matrix, scene.camera->sixth.matrix, scene.camera->seventh.matrix,
                                                       (scene.timer-scene.camera->fifth.t) / (scene.camera->sixth.t - scene.camera->seventh.t));
                scene.timer = 0.0f;
            }

            scene.timer += dt;
        }
    }
    else{
        initSecondScene();
        scene.secondScene = true;
        scene.timer = 0.0f;
    }
  }
};



int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
