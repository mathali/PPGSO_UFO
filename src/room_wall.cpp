#include "room_wall.h"
#include "scene.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Square objects used to build the room that houses cows in the second scene
Room_wall::Room_wall() {
    position = {0, -12, 0};
    rotation = {-ppgso::PI/2, 0, 0};
    scale = {20, 20, 1};
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("room_wall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Room_wall::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Room_wall::render(Scene &scene) {
    shader->use();

    shader->setUniform("LightDirection", {-3.0f, -9.0f, 0.0f});
    shader->setUniform("Ambient", scene.ambient);
    shader->setUniform("SpecularStrength", scene.specularStrength);
    shader->setUniform("diffuse_strength", scene.diffuseStrength);
    shader->setUniform("ViewPosition", scene.camera->position);
    shader->setUniform("Ka", Ka);
    shader->setUniform("Kd", Kd);
    shader->setUniform("Ks", Ks);
    shader->setUniform("object", 5);

    shader->setUniform("BeamFlag", false);
    shader->setUniform("BeamPos", scene.beam_pos);
    shader->setUniform("BeamDirection", scene.beam_direction);
    shader->setUniform("BeamColor", scene.beam_color);
    shader->setUniform("BeamCutoff", scene.beam_cutoff);


    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

// shared resources
std::unique_ptr<ppgso::Mesh> Room_wall::mesh;
std::unique_ptr<ppgso::Shader> Room_wall::shader;
std::unique_ptr<ppgso::Texture> Room_wall::texture;
