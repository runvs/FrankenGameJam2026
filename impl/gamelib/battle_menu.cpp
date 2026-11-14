#include "battle_menu.hpp"

#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "state_encounter.hpp"
#include "tweens/tween_color.hpp"
#include <enemy.hpp>
#include <game_properties.hpp>
#include <state_encounter.hpp>

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
    m_menu1TextAttack->setColor(GP::BattleMenuStateSelectedColor());

    m_menu1TextItem = jt::dh::createText(renderTarget(), "Item", 16u);
    m_menu1TextItem->setTextAlign(jt::Text::TextAlign::LEFT);
    m_menu1TextItem->setOffset(jt::Vector2f { 0.0f, -8.0f });
    m_menu1TextItem->setPosition({ 28.0f, 20.0f + 20.0f * 1.0f });

    m_menu2Text0 = jt::dh::createText(renderTarget(), "Melee", 16u);
    m_menu2Text0->setTextAlign(jt::Text::TextAlign::LEFT);
    m_menu2Text0->setOffset(jt::Vector2f { 0.0f, -8.0f });
    m_menu2Text0->setPosition({ 130.0f, 20.0f + 20.0f * 0.0f });
    m_menu2Text0->setColor(GP::BattleMenuStateSelectedColor());

    m_menu2Text1 = jt::dh::createText(renderTarget(), "Pistol", 16u);
    m_menu2Text1->setTextAlign(jt::Text::TextAlign::LEFT);
    m_menu2Text1->setOffset(jt::Vector2f { 0.0f, -8.0f });
    m_menu2Text1->setPosition({ 130.0f, 20.0f + 20.0f * 1.0f });

    m_selector1 = std::make_shared<jt::Shape>();
    m_selector1->makeCircle(2.0f, textureManager());
    m_selector1->setPosition({ 22.0f, 23.0f });
    m_selector1->setColor(GP::BattleMenuSelectorColor());
    m_selector1->setOffset(jt::OffsetMode::CENTER);

    m_selector2 = std::make_shared<jt::Shape>();
    m_selector2->makeCircle(2.0f, textureManager());
    m_selector2->setPosition({ 122.0f, 23.0f });
    m_selector2->setColor(GP::BattleMenuSelectorColor());
    m_selector2->setOffset(jt::OffsetMode::CENTER);
}

void BattleMenu::doMenu1Tweens()
{
    if (m_selectorMenu1Index == 0) {
        auto tw1 = jt::TweenColor::create(m_menu1TextAttack, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateBaseColor(), GP::BattleMenuStateSelectedColor());
        m_addTween(tw1);

        auto tw2 = jt::TweenColor::create(m_menu1TextItem, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateSelectedColor(), GP::BattleMenuStateBaseColor());
        m_addTween(tw2);

    } else if (m_selectorMenu1Index == 1) {
        auto tw1 = jt::TweenColor::create(m_menu1TextItem, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateBaseColor(), GP::BattleMenuStateSelectedColor());
        m_addTween(tw1);

        auto tw2 = jt::TweenColor::create(m_menu1TextAttack, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateSelectedColor(), GP::BattleMenuStateBaseColor());
        m_addTween(tw2);
    }
}

void BattleMenu::doMenu2Tweens()
{
    if (m_selectorMenu2Index == 0) {
        auto tw1 = jt::TweenColor::create(m_menu2Text0, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateBaseColor(), GP::BattleMenuStateSelectedColor());
        m_addTween(tw1);

        auto tw2 = jt::TweenColor::create(m_menu2Text1, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateSelectedColor(), GP::BattleMenuStateBaseColor());
        m_addTween(tw2);

    } else if (m_selectorMenu2Index == 1) {
        auto tw1 = jt::TweenColor::create(m_menu2Text1, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateBaseColor(), GP::BattleMenuStateSelectedColor());
        m_addTween(tw1);

        auto tw2 = jt::TweenColor::create(m_menu2Text0, GP::BattleMenuTweenTime(),
            GP::BattleMenuStateSelectedColor(), GP::BattleMenuStateBaseColor());
        m_addTween(tw2);
    }
}

void BattleMenu::doUpdate(float const elapsed)
{
    auto const& kb = getGame()->input().keyboard();
    if (m_state == BattleMenuState::Menu1) {
        bool anyPressed = false;
        if (kb->justReleased(jt::KeyCode::W) || kb->justReleased(jt::KeyCode::Up)) {
            m_selectorMenu1Index--;
            if (m_selectorMenu1Index < 0) {
                m_selectorMenu1Index = maxIndex1;
            }
            anyPressed = true;
        }
        if (kb->justReleased(jt::KeyCode::S) || kb->justReleased(jt::KeyCode::Down)) {
            m_selectorMenu1Index++;
            if (m_selectorMenu1Index > maxIndex1) {
                m_selectorMenu1Index = 0;
            }
            anyPressed = true;
        }

        if (anyPressed) {
            doMenu1Tweens();
        }

        if (kb->justReleased(jt::KeyCode::Space)) {
            m_state = BattleMenuState::Menu2;
            if (m_selectorMenu1Index == 0) {
                m_menu2Text0->setText("Melee");
                m_menu2Text1->setText("Pistol");
                // TODO check available options based on player inventory
            } else if (m_selectorMenu1Index == 1) {
                m_menu2Text0->setText("Medkit");

                m_menu2Text1->setText("Molotov");
                // TODO check available options based on player inventory
            }
        }
    } else if (m_state == BattleMenuState::Menu2) {

        if (kb->justReleased(jt::KeyCode::Space)) {
            if (m_selectorMenu1Index == 0) {
                if (m_selectorMenu2Index == 0) {
                    auto const enemy = m_stateEncounter->m_enemies->at(0).lock();
                    enemy->getDamage(5.0f);
                    // std::cout << "!!!!!trigger next actor from battleMenu\n";
                    m_triggerNextActor();
                }
            } else if (m_selectorMenu1Index == 1) {
                // TODO limit number of available items so they cannot be reused over and over again
                if (m_selectorMenu2Index == 0) {
                    m_stateEncounter->damagePlayer(-4.0f);
                    m_triggerNextActor();
                } else if (m_selectorMenu2Index == 1) {
                    // Molotov
                    for (auto const& e : *m_stateEncounter->m_enemies) {
                        auto const enemy = e.lock();
                        enemy->getDamage(3.0f);
                    }
                    m_triggerNextActor();
                }
            }
        }
        if (kb->justReleased(jt::KeyCode::Escape)) {
            m_state = BattleMenuState::Menu1;
        }
        bool anyPressed = false;
        if (kb->justReleased(jt::KeyCode::W) || kb->justReleased(jt::KeyCode::Up)) {
            m_selectorMenu2Index--;
            if (m_selectorMenu2Index < 0) {

                m_selectorMenu2Index = maxIndex2;
            }

            anyPressed = true;
        }
        if (kb->justReleased(jt::KeyCode::S) || kb->justReleased(jt::KeyCode::Down)) {
            m_selectorMenu2Index++;
            if (m_selectorMenu2Index > maxIndex2) {
                m_selectorMenu2Index = 0;
            }
            anyPressed = true;
        }
        if (anyPressed) {
            doMenu2Tweens();
        }
    }

    m_backgroundMenu1->update(elapsed);
    m_backgroundMenu2->update(elapsed);

    m_menu1TextAttack->update(elapsed);
    m_menu1TextItem->update(elapsed);

    m_menu2Text0->update(elapsed);
    m_menu2Text1->update(elapsed);

    m_selector1->setPosition(jt::Vector2f { 22.0f, 23.0f + 20.0f * m_selectorMenu1Index });
    m_selector1->update(elapsed);

    m_selector2->setPosition(jt::Vector2f { 122.0f, 23.0f + 20.0f * m_selectorMenu2Index });
    m_selector2->update(elapsed);
}

void BattleMenu::doDraw() const
{
    if (m_state == BattleMenuState::Menu1) {
        m_backgroundMenu1->draw(renderTarget());

        m_menu1TextAttack->draw(renderTarget());
        m_menu1TextItem->draw(renderTarget());

        m_selector1->draw(renderTarget());
    } else if (m_state == BattleMenuState::Menu2) {
        m_backgroundMenu1->draw(renderTarget());
        m_backgroundMenu2->draw(renderTarget());

        m_menu1TextAttack->draw(renderTarget());
        m_menu1TextItem->draw(renderTarget());

        m_menu2Text0->draw(renderTarget());
        m_menu2Text1->draw(renderTarget());

        m_selector1->draw(renderTarget());
        m_selector2->draw(renderTarget());
    }
}
