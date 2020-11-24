#pragma once

#include <ppgso/ppgso.h>

#include "object.h"

/*!
 * This object renders the scene background
 * It does not use the camera so it uses different 2D shader program
 * Background animation is achieved by passing an offset to the fragment shader that offsets texture mapping
 */
class Room_wall final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
    glm::vec3 Ka = {0.01882, 0.01882, 0.01882};
    glm::vec3 Kd = {0.05882, 0.05882, 0.05882};
    glm::vec3 Ks = {0.009, 0.009, 0.009};
    glm::vec2 textureOffset;
public:
    /*!
     * Create new Space background
     */
    Room_wall();

    /*!
     * Update space background
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render space background
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};

