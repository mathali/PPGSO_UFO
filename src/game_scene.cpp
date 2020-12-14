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

int count = 0;

// Create main window to render the game in
class SceneWindow : public ppgso::Window {
private:
  Scene scene;
  bool animate = true;

  /*
   * First Scene
   * Control a UFO and abduct cows
   */
  void initScene() {
    scene.scene1 = true;
    scene.secondScene = false;
    scene.captured = 0;
    scene.objects.clear();
    scene.timer = 0.0f;

    // Create a camera with a perspective projection
    auto camera = std::make_unique<Camera>(75.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -15.0f;
    camera->position.y = 7.0f;
    camera->secondScene = false;
    scene.camera = move(camera);


    // Create square floor objects that are moved around as the player moves
    float ground_pos;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            auto ground = std::make_unique<Ground>();
            ground->position = {j*40 - 80, -10.8, i*40 - 40};
            ground_pos = ground->position.y;
            scene.objects.push_back(move(ground));
        }
    }


    // Create a generator responsibly for the procedural generation of most objects in the scene
    auto generator = std::make_unique<Generator>();
    generator->position = {0, ground_pos + 0.8f, 0};
    scene.objects.push_back(move(generator));

    // Create a UFO object to represent the player
    auto player = std::make_unique<Player>();
    player->position.y = -3;
    scene.objects.push_back(move(player));
  }

  /*
   * Second Scene
   * Look at the display of cows the player has abducted. Camera is controlled using keyframes
   */
  void initSecondScene(){
      scene.scene1 = false;
      scene.objects.clear();            // Destroy everything from the previous scene
      scene.player_pos.x = 0;
      scene.player_pos.z = 0;

      // Recreate a camera, same parameters as last time, but different position
      auto camera = std::make_unique<Camera>(75.0f, 1.0f, 0.1f, 100.0f);
      camera->position.z = -20.0f;
      camera->position.x = 0.0f;
      camera->position.y = 5.0f;
      camera->secondScene = true;
      camera->back = {camera->position.x, 0, camera->position.z};
      scene.camera = move(camera);

      // Generate cages that are filled with the abducted cows
      for(int j = 0; j < 4; j++)
          for(int i = 0; i < 7; i++) {
              auto cage = std::make_unique<Cage>();
              cage->position = {15 + -5*i, 1, j*10 - 15};
              scene.objects.push_back(move(cage));

              if(scene.captured > 0) {  // Fill cages until they are all filled or we run out of cows
                  auto cow = std::make_unique<Cow>();
                  cow->position = {15 + -5 * i + 2.2, 3.5, j * 10 - 15 - 2.2};
                  cow->scale = {0.5, 0.5, 0.5};
                  cow->rotation = {0, 0, 0};
                  scene.objects.push_back(move(cow));
                  scene.captured--;
              }
          }

      // Create a room for the cages.
      // Created using seperate wall objects - I couldn't find a fitting free model so I had to do it this way.
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

  // Consturctor from gl9, slightly modified parameters
  SceneWindow() : Window{"game_scene", 1280, 720} {
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

  // Modified onKey function used for overall scene controls
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    // Use numbers to change the color of the beam and the light it emits
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        scene.beam_color = {0, 1, 0};
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        scene.beam_color = {1, 0, 0};
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        scene.beam_color = {0, 0, 1};
    }

    // Move the camera up and down
    if (key == GLFW_KEY_UP) {
        scene.camera->position.z += 0.1f;
        scene.camera->position.y -= 0.1f;
    }

    if (key == GLFW_KEY_DOWN) {
        scene.camera->position.z -= 0.1f;
        scene.camera->position.y += 0.1f;
    }

    /*// Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }*/
  }

  // Interpolate keyFrame matrices on a bezier curve - used for second scene camera animation
  glm::mat4 bezierPoint(const glm::mat4 CP0,const glm::mat4 CP1,const glm::mat4 CP2 , float t) {
      glm::mat4 out;
      glm::mat4 supp1_1 = glm::interpolate(CP0, CP1, t);
      glm::mat4 supp1_2 = glm::interpolate(CP1, CP2, t);
      out = glm::interpolate(supp1_1, supp1_2, t);

      return out;
  }

  // Modified gl9 on idle to represent the functionality of this games 2 scenes
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


    // Update and render all objects if we are in the second scene or the first scene's time limit hasn't run out
    if(scene.timer < 60.0f || scene.secondScene) {
        scene.update(dt);
        scene.render();
        // Handle the camera's keyframe animation if we are rendering the second scene
        if(scene.secondScene){
            // Calculate a point on a bezier curve based on keyframe structs that belong to the camera object
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
    // Switch scenes
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
