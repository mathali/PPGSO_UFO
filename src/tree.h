#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Simple house object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 */
class Tree final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Age of the object in seconds
    float age{0.0f};
    glm::vec3 Ka = {0.036861, 0.036861, 0.036861};
    glm::vec3 Kd = {3.640506, 3.640506, 3.640506};
    glm::vec3 Ks = {0.210769, 0.210769, 0.210769};
    /*!
     * Split the asteroid into multiple pieces and spawn an explosion object.
     *
     * @param scene - Scene to place pieces and explosion into
     * @param explosionPosition - Initial position of the explosion
     * @param explosionScale - Scale of the explosion
     * @param pieces - Asteroid pieces to generate
     */

public:
    /*!
     * Create new asteroid
     */
    Tree();

    /*!
     * Update asteroid
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

private:
};

