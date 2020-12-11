#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "cow.h"

/*!
 * Simple house object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 */
class Cow_shadow final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    std::shared_ptr<Cow> parent;

public:
    bool caught = false;
    /*!
     * Create new asteroid
     */
    Cow_shadow(std::shared_ptr<Cow> par);

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

// Age of the object in seconds
    float age{0.0f};
private:
};
