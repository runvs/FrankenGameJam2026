#include "player.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <vector.hpp>

Player::Player(std::shared_ptr<jt::Box2DWorldInterface> world)
{
    auto bodyDef = b2DefaultBodyDef();
    bodyDef.motionLocks.angularZ = true;
    bodyDef.type = b2_dynamicBody;

    m_physicsObject = std::make_shared<jt::Box2DObject>(world, bodyDef);
}

void Player::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();

    m_animation->loadFromAseprite("assets/chars.aseprite", textureManager());
    m_animation->play("idle");
    m_animation->setOrigin(jt::OriginMode::CENTER);
    m_animation->setZ(GP::LevelZLayerPlayer());

    auto const bodyId = m_physicsObject->getBodyId();

    // TODO check player collision correct?
    auto colliderDef = b2DefaultShapeDef();
    colliderDef.density = 1.0f;
    colliderDef.material.friction = 0.5f;
    // contact events are needed to detect collisions with enemies
    colliderDef.enableContactEvents = true;
    colliderDef.filter.categoryBits = GP::CollisionBitPlayer();
    b2Circle const circleCollider { b2Vec2 { 0.0f, 0.0f }, 12.0f };
    b2CreateCircleShape(bodyId, &colliderDef, &circleCollider);
}

std::shared_ptr<jt::Animation> Player::getAnimation() { return m_animation; }

void Player::doUpdate(float const elapsed)
{
    auto const currentPosition = m_physicsObject->getPosition();
    m_animation->setPosition(currentPosition);
    updateAnimation(elapsed);
    handleMovement(elapsed);
}

void Player::updateAnimation(float const elapsed)
{
    // TODO play animation based on movement
    m_animation->update(elapsed);

    if (getGame()->input().keyboard()->pressed(jt::KeyCode::D)
        || getGame()->input().keyboard()->pressed(jt::KeyCode::Right)) {
        m_animation->setScale({ -1.0f, 1.0f });
    }

    if (getGame()->input().keyboard()->pressed(jt::KeyCode::A)
        || getGame()->input().keyboard()->pressed(jt::KeyCode::Left)) {
        m_animation->setScale({ 1.0f, 1.0f });
    }
}

void Player::handleMovement(float const elapsed)
{
    // TODO check input and set internal physics/state/position
    jt::Vector2f newVelocity { 0.0f, 0.0f };

    float const speed = GP::PlayerBaseVelocity();
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::D)
        || getGame()->input().keyboard()->pressed(jt::KeyCode::Right)) {
        newVelocity += jt::Vector2f { speed, 0.0f };
    }
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::A)
        || getGame()->input().keyboard()->pressed(jt::KeyCode::Left)) {
        newVelocity += jt::Vector2f { -speed, 0.0f };
    }

    if (getGame()->input().keyboard()->pressed(jt::KeyCode::W)
        || getGame()->input().keyboard()->pressed(jt::KeyCode::Up)) {
        newVelocity += jt::Vector2f { 0.0f, -speed };
    }
    if (getGame()->input().keyboard()->pressed(jt::KeyCode::S)
        || getGame()->input().keyboard()->pressed(jt::KeyCode::Down)) {
        newVelocity += jt::Vector2f { 0.0f, speed };
    }

    // physObject.setVelocity(newVelocity);
    m_physicsObject->setVelocity(newVelocity);
}

void Player::doDraw() const { m_animation->draw(renderTarget()); }

jt::Vector2f Player::getPosOnScreen() const { return m_animation->getScreenPosition(); }

void Player::setPosition(jt::Vector2f const& pos) { m_physicsObject->setPosition(pos); }

jt::Vector2f Player::getPosition() const { return m_physicsObject->getPosition(); }
