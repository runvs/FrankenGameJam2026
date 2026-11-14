#pragma once

#include "encounter/level_encounter.hpp"
#include "level.hpp"
#include "object_group.hpp"
#include "player.hpp"
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
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

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    bool m_running { true };
    bool m_hasEnded { false };

    std::shared_ptr<Level> m_level { nullptr };

    std::shared_ptr<Player> m_player { nullptr };

    std::shared_ptr<jt::ObjectGroup<LevelEncounter>> m_levelEncounters;

    void onCreate() override;
    void onEnter() override;
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();

    void loadLevel();
    void updateLevelEncounters(float elapsed);
};
