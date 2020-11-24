#include "space.h"
#include "scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>



Space::Space() {
  // Initialize static resources if needed

  position = {0, -12, 0};
  rotation = {-ppgso::PI/2, 0, 0};
  scale = {20, 20, 1};
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("grass8.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Space::update(Scene &scene, float dt) {
  // Offset for UV mapping, creates illusion of scrolling
  //textureOffset.y -= dt/5;
  //position.y = -0.5;
  //rotation = {1, 0, 0};
  //scale *= 1.f;
  generateModelMatrix();
  return true;
}

void Space::render(Scene &scene) {
  // Disable writing to the depth buffer so we render a "background"
  //glDepthMask(GL_FALSE);

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("Ambient", scene.ambient);
    shader->setUniform("SpecularStrength", scene.specularStrength);
    shader->setUniform("diffuse_strength", scene.diffuseStrength);
    shader->setUniform("ViewPosition", scene.camera->position);
    shader->setUniform("Ka", Ka);
    shader->setUniform("Kd", Kd);
    shader->setUniform("Ks", Ks);
    shader->setUniform("object", 5);

    shader->setUniform("BeamFlag", scene.beam_flag);
    shader->setUniform("BeamPos", scene.beam_pos);
    shader->setUniform("BeamDirection", scene.beam_direction);
    shader->setUniform("BeamColor", scene.beam_color);
    shader->setUniform("BeamCutoff", scene.beam_cutoff);


    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();

  //glDepthMask(GL_TRUE);
}

// shared resources
std::unique_ptr<ppgso::Mesh> Space::mesh;
std::unique_ptr<ppgso::Shader> Space::shader;
std::unique_ptr<ppgso::Texture> Space::texture;
