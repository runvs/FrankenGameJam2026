#include "battle_menu.hpp"

#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "state_encounter.hpp"
#include "state_game.hpp"
#include "state_menu.hpp"
#include "tweens/tween_color.hpp"
#include <enemy.hpp>
#include <game_properties.hpp>
#include <state_encounter.hpp>
#include <state_game.hpp>
#include <format>

constexpr int maxIndex1 = 1;

void BattleMenu::doCreate()
{
    m_backgroundMenu1
        = std::make_shared<jt::Sprite>("assets/battle_menu_background.png", textureManager());
    m_backgroundMenu1->setPosition({ 16.0f, 12.0f });

    m_backgroundMenu2
        = std::make_shared<jt::Sprite>("assets/battle_menu_background2.png", textureManager());
    m_backgroundMenu2->setPosition({ 112.0f, 12.0f });

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
    m_menu2Text0->setPosition({ 124.0f, 20.0f + 20.0f * 0.0f });
    m_menu2Text0->setColor(GP::BattleMenuStateSelectedColor());

    m_menu2Text1 = jt::dh::createText(renderTarget(), "Pistol", 16u);
    m_menu2Text1->setTextAlign(jt::Text::TextAlign::LEFT);
    m_menu2Text1->setOffset(jt::Vector2f { 0.0f, -8.0f });
    m_menu2Text1->setPosition({ 124.0f, 20.0f + 20.0f * 1.0f });

    m_selector1 = std::make_shared<jt::Shape>();
    m_selector1->makeCircle(2.0f, textureManager());
    m_selector1->setPosition({ 22.0f, 23.0f });
    m_selector1->setColor(GP::BattleMenuSelectorColor());
    m_selector1->setOffset(jt::OffsetMode::CENTER);

    m_selector2 = std::make_shared<jt::Shape>();
    m_selector2->makeCircle(2.0f, textureManager());
    m_selector2->setPosition({ 120.0f, 23.0f });
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

    // count down time for space timer
    if (m_SpaceWasPressedTimer > 0) {
        m_SpaceWasPressedTimer -= elapsed;
    } else {
        m_SpaceWasPressedTimer = 0;
    }

    // while
    // (!m_stateEncounter->m_enemies->at(m_chosen_enemy).lock()->isAlive()) {
    //    m_chosen_enemy++;
    //};

    if (m_state == BattleMenuState::Menu1) {

        // unpaint all enemies and hero
        m_stateEncounter->m_player->setColor(jt::colors::White);

        for (auto const& e : *m_stateEncounter->m_enemies) {
            auto const enemy = e.lock();
            enemy->isChosen(false);
        }

        bool anyPressed = false;

        if (kb->justReleased(jt::KeyCode::W) || kb->justReleased(jt::KeyCode::Up)) {
            auto snd = getGame()->audio().addTemporarySound("event:/menu-select");
            snd->play();
            m_selectorMenu1Index--;
            if (m_selectorMenu1Index < 0) {
                m_selectorMenu1Index = maxIndex1;
            }
            anyPressed = true;
        }
        if (kb->justReleased(jt::KeyCode::S) || kb->justReleased(jt::KeyCode::Down)) {
            auto snd = getGame()->audio().addTemporarySound("event:/menu-select");
            snd->play();
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
            auto snd = getGame()->audio().addTemporarySound("event:/menu-space-bar");
            snd->play();
            m_state = BattleMenuState::Menu2;
            if (m_selectorMenu1Index == 0) {
                m_menu2Text0->setText("Melee");

                m_menu2Text1->setText("Shotgun");
                // TODO check available options based on player inventory
            } else if (m_selectorMenu1Index == 1) {

                m_menu2Text0->setText(std::format(
                    "Medkit [{}]", m_stateEncounter->m_stateGame->m_playerInfo.m_nMedKits));
                // change color to gray n = 0
                if (m_stateEncounter->m_stateGame->m_playerInfo.m_nMedKits == 0) {
                    m_menu2Text0->setColor(jt::colors::Gray);
                }

                m_menu2Text1->setText(std::format(
                    "Molotov [{}]", m_stateEncounter->m_stateGame->m_playerInfo.m_nMolotov));
                if (m_stateEncounter->m_stateGame->m_playerInfo.m_nMolotov == 0) {
                    m_menu2Text1->setColor(jt::colors::Gray);
                }
                // TODO check available options based on player inventory
            }
        }
    } else if (m_state == BattleMenuState::Menu2) {

        if (m_selectorMenu1Index == 0) {
            if (m_selectorMenu2Index == 0) {

                // unpaint all enemies and hero
                m_stateEncounter->m_player->setColor(jt::colors::White);

                for (auto const& e : *m_stateEncounter->m_enemies) {
                    auto const enemy = e.lock();
                    enemy->isChosen(false);
                }

                if (kb->justReleased(jt::KeyCode::D) || kb->justReleased(jt::KeyCode::Right)) {
                    auto snd = getGame()->audio().addTemporarySound("event:/menu-select");
                    snd->play();
                    m_chosen_enemy++;
                    if (m_chosen_enemy >= m_stateEncounter->m_enemies->size()) {
                        m_chosen_enemy = 0;
                    }
                }
                if (m_chosen_enemy < m_stateEncounter->m_enemies->size()) {
                    auto const enemy = m_stateEncounter->m_enemies->at(m_chosen_enemy).lock();
                    enemy->isChosen(true);
                }
                if (kb->justReleased(jt::KeyCode::A) || kb->justReleased(jt::KeyCode::Left)) {
                    auto snd = getGame()->audio().addTemporarySound("event:/menu-select");
                    snd->play();
                    m_chosen_enemy--;
                    if (m_chosen_enemy < 0) {
                        m_chosen_enemy = m_stateEncounter->m_enemies->size() - 1;
                    }
                }
                if (m_chosen_enemy < m_stateEncounter->m_enemies->size()) {
                    auto const enemy = m_stateEncounter->m_enemies->at(m_chosen_enemy).lock();
                    enemy->isChosen(true);
                }

            } else if (m_selectorMenu2Index == 1) {
                m_stateEncounter->m_player->setColor(jt::colors::White);
                for (auto const& e : *m_stateEncounter->m_enemies) {
                    auto const enemy = e.lock();
                    enemy->isChosen(true);
                }
            }
        } else if (m_selectorMenu1Index == 1) {
            if (m_selectorMenu2Index == 0) {
                m_stateEncounter->m_player->setColor(jt::colors::Green);
                for (auto const& e : *m_stateEncounter->m_enemies) {
                    auto const enemy = e.lock();
                    enemy->isChosen(false);
                }
            } else if (m_selectorMenu2Index == 1) {
                m_stateEncounter->m_player->setColor(jt::colors::White);
                for (auto const& e : *m_stateEncounter->m_enemies) {
                    auto const enemy = e.lock();
                    enemy->isChosen(true);
                }
            }
        }

        if (kb->justReleased(jt::KeyCode::Space) && m_SpaceWasPressedTimer <= 0) {

            if (m_selectorMenu1Index == 0) {
                if (m_selectorMenu2Index == 0) {

                    // Melee
                    auto snd = getGame()->audio().addTemporarySound("event:/attack-melee");
                    snd->play();
                    auto const enemy = m_stateEncounter->m_enemies->at(m_chosen_enemy).lock();
                    enemy->getDamage(Damage { 5.0f, DamageType::Normal });

                    // std::cout << "!!!!!trigger next actor from battleMenu\n";
                    if (!enemy->isAlive()) {
                        m_chosen_enemy = 0;
                    }
                    m_state = BattleMenuState::Menu1;
                    m_SpaceWasPressedTimer = GP::BattleSpaceWasPressedTimer();
                    m_triggerNextActor();

                } else if (m_selectorMenu2Index == 1) {
                    // Shotgun

                    auto snd = getGame()->audio().addTemporarySound("event:/attack-shotgun");
                    snd->play();

                    for (auto const& e : *m_stateEncounter->m_enemies) {
                        auto const enemy = e.lock();
                        enemy->getDamage(Damage { 1.0f, DamageType::Normal });
                        enemy->isChosen(false);
                    }
                    m_state = BattleMenuState::Menu1;
                    m_SpaceWasPressedTimer = GP::BattleSpaceWasPressedTimer();
                    m_triggerNextActor();
                }
            } else if (m_selectorMenu1Index == 1) {
                // TODO limit number of available items so they cannot be reused
                // over and over again
                if (m_selectorMenu2Index == 0) {

                    // Healing
                    auto snd = getGame()->audio().addTemporarySound("event:/item-heal");
                    snd->play();
                    if (m_stateEncounter->m_stateGame->m_playerInfo.m_nMedKits > 0) {
                        m_stateEncounter->damagePlayer(Damage { -10.0f, DamageType::Heal });
                        m_stateEncounter->m_stateGame->m_playerInfo.m_nMedKits -= 1;
                        m_state = BattleMenuState::Menu1;
                        m_SpaceWasPressedTimer = GP::BattleSpaceWasPressedTimer();
                        m_triggerNextActor();
                    } else {
                        auto snd = getGame()->audio().addTemporarySound("event:/menu-error");
                        snd->play();
                    }
                } else if (m_selectorMenu2Index == 1) {
                    // Molotov
                    auto snd = getGame()->audio().addTemporarySound("event:/item-molotov");
                    snd->play();

                    if (m_stateEncounter->m_stateGame->m_playerInfo.m_nMolotov > 0) {
                        for (auto const& e : *m_stateEncounter->m_enemies) {
                            auto const enemy = e.lock();
                            enemy->getDamage(Damage { GP::MolotovDamage(), DamageType::Fire });
                        }
                        m_stateEncounter->m_stateGame->m_playerInfo.m_nMolotov -= 1;
                        m_state = BattleMenuState::Menu1;
                        m_SpaceWasPressedTimer = GP::BattleSpaceWasPressedTimer();
                        m_triggerNextActor();
                    } else {
                        auto snd = getGame()->audio().addTemporarySound("event:/menu-error");
                        snd->play();
                    }
                }
            }
        }
        if (kb->justReleased(jt::KeyCode::Escape)) {
            auto snd = getGame()->audio().addTemporarySound("event:/menu-error");
            snd->play();
            m_state = BattleMenuState::Menu1;
            m_stateEncounter->m_player->setColor(jt::colors::White);
            for (auto const& e : *m_stateEncounter->m_enemies) {
                auto const enemy = e.lock();
                enemy->isChosen(false);
            }
        }
        bool anyPressed = false;
        if (kb->justReleased(jt::KeyCode::W) || kb->justReleased(jt::KeyCode::Up)) {
            auto snd = getGame()->audio().addTemporarySound("event:/menu-select");
            snd->play();
            m_selectorMenu2Index--;
            if (m_selectorMenu2Index < 0) {

                m_selectorMenu2Index = maxIndex2;
            }

            anyPressed = true;
        }
        if (kb->justReleased(jt::KeyCode::S) || kb->justReleased(jt::KeyCode::Down)) {
            auto snd = getGame()->audio().addTemporarySound("event:/menu-select");
            snd->play();
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

    m_selector2->setPosition(jt::Vector2f { 120.0f, 23.0f + 20.0f * m_selectorMenu2Index });
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
