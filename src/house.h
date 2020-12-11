#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "house_shadow.h"

/*!
 * Simple house object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 */
class House final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Age of the object in seconds
    float age{0.0f};

    glm::vec3 Ka = {0.024155, 0.024155, 0.024155};
    glm::vec3 Kd = {0.737255, 0.737255, 0.737255};
    glm::vec3 Ks = {0.034077, 0.034077, 0.034077};



public:
    House_shadow* shadow;
    /*!
     * Create new asteroid
     */
    House();

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

    /*!
     * Custom click event for asteroid
     */
    void onClick(Scene &scene) override;

    /*!
     * Split the asteroid into multiple pieces and spawn an explosion object.
     *
     * @param scene - Scene to place pieces and explosion into
     * @param explosionPosition - Initial position of the explosion
     * @param explosionScale - Scale of the explosion
     * @param pieces - Asteroid pieces to generate
     */
    void explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces);

private:
};

