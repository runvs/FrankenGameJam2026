#pragma once

#include "encounter/level_encounter.hpp"
#include "level.hpp"
#include "level_pickup.hpp"
#include "object_group.hpp"
#include "player.hpp"
#include "player_info.hpp"
#include "screeneffects/stars.hpp"
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
#include <text.hpp>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
class Vignette;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    std::string getName() const override;

    PlayerInfo m_playerInfo;

    bool m_running { true };

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Stars> m_stars { nullptr };
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    bool m_hasEnded { false };

    std::shared_ptr<Level> m_level { nullptr };

    std::shared_ptr<Player> m_player { nullptr };

    std::shared_ptr<jt::ObjectGroup<LevelEncounter>> m_levelEncounters;
    std::shared_ptr<jt::ObjectGroup<LevelPickup>> m_levelPickups;

    void onCreate() override;
    void onEnter() override;
    void onUpdate(float elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();
    void cleanUpLoadedLevel();

    void loadLevel(std::string const& levelName = "assets/level_1.json");

    void updateLevelEncounters(float elapsed);
    void handleDebugInput();
    void updateExit(jt::tilemap::InfoRect const& o);

    std::shared_ptr<jt::Sprite> m_medkitSprite;
    jt::Vector2f m_medkitPosition { 220.0f, 220.0f };
    std::shared_ptr<jt::Text> m_medkitText;

    std::shared_ptr<jt::Sprite> m_molotovSprite;
    jt::Vector2f m_molotovPosition { 260.0f, 220.0f };
    std::shared_ptr<jt::Text> m_molotovText;
};
