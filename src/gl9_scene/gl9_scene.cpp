// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "player.h"
#include "space.h"
#include "asteroid.h"
#include "room_wall.h"
#include "cage.h"

const unsigned int SIZE = 512;

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
    scene.camera = move(camera);

    // Add space background

    float space_pos;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            auto space = std::make_unique<Space>();
            space->position = {j*40 - 80, -10.8, i*40 - 40};
            space_pos = space->position.y;
            scene.objects.push_back(move(space));
        }
    }


    // Add generator to scene
    auto generator = std::make_unique<Generator>();
    generator->position = {0, space_pos + 0.8f, 0};
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
      camera->back = {camera->position.x, 0, camera->position.z};
      scene.camera = move(camera);

      auto floor = std::make_unique<Room_wall>();
      floor->position = {0, 0, 0};
      //floor->scale = {2, 2, 1};
      scene.objects.push_back(move(floor));

      auto ceiling = std::make_unique<Room_wall>();
      ceiling->position = {0, 20, 0};
      ceiling->rotation = {-3*ppgso::PI/2, 0, 0};
      scene.objects.push_back(move(ceiling));

      auto side1 = std::make_unique<Room_wall>();
      side1->position = {0, 10, 20};
      side1->rotation = {-ppgso::PI, 0, 0};
      //side1->scale = {2, 2, 1};
      scene.objects.push_back(move(side1));

      auto side2 = std::make_unique<Room_wall>();
      side2->position = {20, 10, 0};
      side2->rotation = {-ppgso::PI, ppgso::PI, -3*ppgso::PI/2};
      //side2->scale = {5, 5, 1};
      scene.objects.push_back(move(side2));

      auto side3 = std::make_unique<Room_wall>();
      side3->position = {-20, 10, 0};
      side3->rotation = {-ppgso::PI, ppgso::PI, 3*ppgso::PI/2};
      scene.objects.push_back(move(side3));

      for(int j = 0; j < 3; j++)
      for(int i = 0; i < 7; i++) {
          auto cage = std::make_unique<Cage>();
          cage->position = {17.5 + -5*i, 1, j*10 - 10};
          //cage->rotation = {-ppgso::PI, ppgso::PI, 3*ppgso::PI/2};
          scene.objects.push_back(move(cage));

          if(scene.captured > 0) {
              auto cow = std::make_unique<Asteroid>();
              cow->position = {17.5 + -5 * i + 2.2, 3.5, j * 10 - 10 - 2.2};
              cow->scale = {0.5, 0.5, 0.5};
              cow->rotation = {0, 0, ppgso::PI};
              scene.objects.push_back(move(cow));
              scene.captured--;
          }
      }

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

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }
  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override {
    scene.cursor.x = cursorX;
    scene.cursor.y = cursorY;
  }

  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      scene.cursor.left = action == GLFW_PRESS;

      if (scene.cursor.left) {
        // Convert pixel coordinates to Screen coordinates
        double u = (scene.cursor.x / width - 0.5f) * 2.0f;
        double v = - (scene.cursor.y / height - 0.5f) * 2.0f;

        // Get mouse pick vector in world coordinates
        auto direction = scene.camera->cast(u, v);
        auto position = scene.camera->position;

        // Get all objects in scene intersected by ray
        auto picked = scene.intersect(position, direction);

        // Go through all objects that have been picked
        for (auto &obj: picked) {
          // Pass on the click event
          obj->onClick(scene);
        }
      }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      scene.cursor.right = action == GLFW_PRESS;
    }
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
    if(scene.timer < 1000.0f || scene.secondScene) {
        scene.update(dt);
        scene.render();
    }
    else{
        initSecondScene();
        scene.secondScene = true;
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
