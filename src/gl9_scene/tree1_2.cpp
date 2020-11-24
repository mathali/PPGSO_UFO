#include <glm/gtc/random.hpp>
#include "tree1_2.h"
#include "player.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Tree1_2::mesh;
std::unique_ptr<ppgso::Texture> Tree1_2::texture;
std::unique_ptr<ppgso::Shader> Tree1_2::shader;

Tree1_2::Tree1_2() {
    // Set random scale speed and rotation
    scale *=  0.5f;//glm::linearRand(1.0f, 3.0f);
    rotation = {-ppgso::PI/2, 0, 0};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("nature/tree1_2.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("nature/tree1_2_test.obj");
}

bool Tree1_2::update(Scene &scene, float dt) {
    // Rotate the object
    //rotation += rotMomentum * dt;

    /*for (auto &obj : scene.objects) {
        if (obj.get() == this) continue;
        auto player = dynamic_cast<Player *>(obj.get());
        if (player && (abs(position.x - player->position.x) >= 80 || abs(position.z - player->position.z >= 80))) {
            return false;
        }
    }*/

    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Tree1_2::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("Ambient", scene.ambient);
    shader->setUniform("SpecularStrength", scene.specularStrength);
    shader->setUniform("ViewPosition", scene.camera->position);
    shader->setUniform("Ka", Ka);
    shader->setUniform("Kd", Kd);
    shader->setUniform("Ks", Ks);


    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Tree1_2::onClick(Scene &scene) {
    std::cout << "Asteroid clicked!" << std::endl;
    //explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
    age = 10000;
}

