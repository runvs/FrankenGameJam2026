#pragma once

#include "level.hpp"
#include "player.hpp"
#include <box2dwrapper/box2d_world_interface.hpp>
#include <encounter/encounter_info.hpp>
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

class StateEncounter : public jt::GameState {
public:
    StateEncounter(EncounterInfo);
    std::string getName() const override;

private:
    EncounterInfo m_encounter_info;
    std::shared_ptr<jt::Sprite> m_background;
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    bool m_running { true };
    bool m_hasEnded { false };

    std::shared_ptr<Level> m_level { nullptr };

    std::shared_ptr<jt::Animation> m_player { nullptr };

    void onCreate() override;
    void onEnter() override;
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();
    void createEnemies();
};
