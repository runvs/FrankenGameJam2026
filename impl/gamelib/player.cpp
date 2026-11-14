#include "player.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>

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
    m_animation->setOffset(jt::OffsetMode::CENTER);
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
}

void Player::handleMovement(float const elapsed)
{
    // TODO check input and set internal state/position
}

void Player::doDraw() const { m_animation->draw(renderTarget()); }

jt::Vector2f Player::getPosOnScreen() const { return m_animation->getScreenPosition(); }

void Player::setPosition(jt::Vector2f const& pos) { m_physicsObject->setPosition(pos); }

jt::Vector2f Player::getPosition() const { return m_physicsObject->getPosition(); }
