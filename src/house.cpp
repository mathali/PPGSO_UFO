#include <glm/gtc/random.hpp>
#include "house.h"
#include "explosion.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


std::unique_ptr<ppgso::Mesh> House::mesh;
std::unique_ptr<ppgso::Texture> House::texture;
std::unique_ptr<ppgso::Shader> House::shader;

// Model to represent the houses/barns in the scene
House::House() {
    scale *=  0.004f;
    rotation = {0, 0, glm::linearRand(-3.0f, 3.0f)};

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("house.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("house.obj");
}

bool House::update(Scene &scene, float dt) {
    if (age > 10.0f) return false;

    generateModelMatrix();

    return true;
}

// Delete the object and create and explosion animation
void House::explode(Scene &scene, glm::vec3 explosionPosition) {
    auto explosion = std::make_unique<Explosion>();
    explosion->position = explosionPosition;
    scene.objects.push_back(move(explosion));
    age = 100.0f;

}

void House::render(Scene &scene) {
    shader->use();


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
