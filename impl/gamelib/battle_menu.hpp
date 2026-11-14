#pragma once

#include "shape.hpp"
#include "text.hpp"
#include <game_object.hpp>

enum class BattleMenuState { Menu1, Menu2 };

class BattleMenu : public jt::GameObject {
public:
    BattleMenuState m_state { BattleMenuState::Menu1 };

private:
    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;

    std::shared_ptr<jt::Shape> m_backgroundMenu1;
    std::shared_ptr<jt::Shape> m_backgroundMenu2;
    std::shared_ptr<jt::Shape> m_selection;

    std::shared_ptr<jt::Text> m_menu1TextAttack;
    std::shared_ptr<jt::Text> m_menu1TextItem;

    int m_selectorMenu1Index { 0 };
};
