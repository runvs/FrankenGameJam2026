#include "battle_menu.hpp"

#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include <game_properties.hpp>

constexpr int maxIndex1 = 1;

void BattleMenu::doCreate()
{
    m_backgroundMenu1 = std::make_shared<jt::Shape>();
    m_backgroundMenu1->makeRect(jt::Vector2f { 80.0f, 90.0f }, textureManager());
    m_backgroundMenu1->setPosition({ 16.0f, 16.0f });
    m_backgroundMenu1->setColor(GP::BattleMenuBackgroundColor());

    m_backgroundMenu2 = std::make_shared<jt::Shape>();
    m_backgroundMenu2->makeRect(jt::Vector2f { 80.0f, 90.0f }, textureManager());
    m_backgroundMenu2->setPosition({ 112.0f, 16.0f });
    m_backgroundMenu2->setColor(GP::BattleMenuBackgroundColor());

    m_menu1TextAttack = jt::dh::createText(renderTarget(), "Attack", 16u);
    m_menu1TextAttack->setTextAlign(jt::Text::TextAlign::LEFT);
    m_menu1TextAttack->setOffset(jt::Vector2f { 0.0f, -8.0f });
    m_menu1TextAttack->setPosition({ 28.0f, 20.0f + 20.0f * 0.0f });

    m_menu1TextItem = jt::dh::createText(renderTarget(), "Item", 16u);
    m_menu1TextItem->setTextAlign(jt::Text::TextAlign::LEFT);
    m_menu1TextItem->setOffset(jt::Vector2f { 0.0f, -8.0f });
    m_menu1TextItem->setPosition({ 28.0f, 20.0f + 20.0f * 1.0f });

    m_selection = std::make_shared<jt::Shape>();
    m_selection->makeCircle(2.0f, textureManager());
    m_selection->setPosition({ 22.0f, 24.0f });
    m_selection->setColor(GP::BattleMenuSelectorColor());
    m_selection->setOffset(jt::OffsetMode::CENTER);
}

void BattleMenu::doUpdate(float const elapsed)
{
    auto const& kb = getGame()->input().keyboard();
    if (m_state == BattleMenuState::Menu1) {
        // TODO tweens
        if (kb->justReleased(jt::KeyCode::W) || kb->justReleased(jt::KeyCode::Up)) {
            m_selectorMenu1Index--;
            if (m_selectorMenu1Index < 0) {

                m_selectorMenu1Index = maxIndex1;
            }
        }
        if (kb->justReleased(jt::KeyCode::S) || kb->justReleased(jt::KeyCode::Down)) {
            m_selectorMenu1Index++;
            if (m_selectorMenu1Index > maxIndex1) {
                m_selectorMenu1Index = 0;
            }
        }
        if (kb->justReleased(jt::KeyCode::Space)) {
            m_state = BattleMenuState::Menu2;
        }
    } else if (m_state == BattleMenuState::Menu2) {
    }

    m_backgroundMenu1->update(elapsed);
    m_backgroundMenu2->update(elapsed);

    m_menu1TextAttack->update(elapsed);
    m_menu1TextItem->update(elapsed);

    m_selection->setPosition(jt::Vector2f { 22.0f, 23.0f + 20.0f * m_selectorMenu1Index });
    m_selection->update(elapsed);
}

void BattleMenu::doDraw() const
{
    if (m_state == BattleMenuState::Menu1) {
        m_backgroundMenu1->draw(renderTarget());
        m_menu1TextAttack->draw(renderTarget());
        m_menu1TextItem->draw(renderTarget());
        m_selection->draw(renderTarget());
    } else if (m_state == BattleMenuState::Menu2) {
        m_backgroundMenu1->draw(renderTarget());
        m_backgroundMenu2->draw(renderTarget());
        m_menu1TextAttack->draw(renderTarget());
        m_menu1TextItem->draw(renderTarget());
        m_selection->draw(renderTarget());
    }
}
