#include <glm/gtc/random.hpp>
#include "house.h"
#include "explosion.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> House::mesh;
std::unique_ptr<ppgso::Texture> House::texture;
std::unique_ptr<ppgso::Shader> House::shader;

House::House() {
    // Set random scale speed and rotation
    scale *=  0.004f;//glm::linearRand(1.0f, 3.0f);
    rotation = {0, 0, glm::linearRand(-3.0f, 3.0f)};


    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("house.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("house.obj");
}

bool House::update(Scene &scene, float dt) {
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
    /*for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            std::cout<< modelMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }*/
    //glm::vec3 test = { modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2] };
    //std::cout << glm::normalize(test * position).x << " " << glm::normalize(test * position).y<< " " << glm::normalize(test * position).z <<std::endl;
    generateModelMatrix();

    return true;
}

void House::explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces) {
    // Generate explosion
    auto explosion = std::make_unique<Explosion>();
    explosion->position = explosionPosition;
    explosion->scale = explosionScale;
    //explosion->speed = speed / 2.0f;
    scene.objects.push_back(move(explosion));
    age = 100.0f;

    /*// Generate smaller asteroids
    for (int i = 0; i < pieces; i++) {
        auto asteroid = std::make_unique<Asteroid>();
        //asteroid->speed = speed + glm::vec3(glm::linearRand(-3.0f, 3.0f), glm::linearRand(0.0f, -5.0f), 0.0f);;
        asteroid->position = position;
        //asteroid->rotMomentum = rotMomentum;
        float factor = (float) pieces / 2.0f;
        asteroid->scale = scale / factor;
        scene.objects.push_back(move(asteroid));
    }*/
}

void House::render(Scene &scene) {
    shader->use();

    // Set up light

    shader->setUniform("BeamFlag", scene.beam_flag);
    shader->setUniform("BeamPos", scene.beam_pos - position);
    shader->setUniform("BeamDirection",  scene.beam_direction);
    shader->setUniform("BeamColor", scene.beam_color);
    shader->setUniform("BeamCutoff", scene.beam_cutoff);
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("Ambient", scene.ambient);
    shader->setUniform("SpecularStrength", scene.specularStrength);
    shader->setUniform("diffuse_strength", scene.diffuseStrength);
    shader->setUniform("ViewPosition", scene.camera->position);
    shader->setUniform("Ka", Ka);
    shader->setUniform("Kd", Kd);
    shader->setUniform("Ks", Ks);
    shader->setUniform("object", 4);



    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
