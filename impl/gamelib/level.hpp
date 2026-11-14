#ifndef JAMTEMPLATE_INVENTORY_LEVEL_HPP
#define JAMTEMPLATE_INVENTORY_LEVEL_HPP

#include <box2dwrapper/box2d_object.hpp>
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_object.hpp>
#include <tilemap/object_layer.hpp>
#include <tilemap/tile_layer.hpp>
#include <tilemap/tileson_loader.hpp>
#include <memory>

class Level : public jt::GameObject {
public:
    Level(std::weak_ptr<jt::Box2DWorldInterface> world);

    void loadTileLayers(jt::tilemap::TilesonLoader& loader);
    void loadLevelCollisions(jt::tilemap::TilesonLoader& loader);

    std::vector<jt::tilemap::InfoRect> getObjects();

    jt::Vector2f getPlayerStartingPos() const;

private:
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerGround;
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerGroundDecals;
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerOverhead;

    std::weak_ptr<jt::Box2DWorldInterface> m_world;
    std::vector<std::shared_ptr<jt::Box2DObject>> m_colliders { };

    std::shared_ptr<jt::tilemap::ObjectLayer> m_invObjectsLayer { nullptr };

    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;
};

#endif // JAMTEMPLATE_INVENTORY_LEVEL_HPP
