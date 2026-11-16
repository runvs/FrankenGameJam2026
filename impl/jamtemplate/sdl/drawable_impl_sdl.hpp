#pragma once

#include <graphics/drawable_impl.hpp>

namespace jt {
class DrawableImplSdl : public jt::DrawableImpl {
public:
    SDL_BlendMode getSDLBlendMode() const noexcept;

    void setOriginInternal(jt::Vector2f const& origin) noexcept override;

    jt::Vector2f getCompleteCamOffset() const noexcept;

protected:
    // Mirrors SFML semantics: a negative scale flips the drawable around its origin. The origin is
    // therefore measured from the opposite edge on a flipped axis.
    void updateOffsetFromOrigin(
        jt::Vector2f const& origin, jt::Vector2f const& unscaledSize) noexcept;

    // Rotation pivot relative to the top left corner of the destination rect.
    SDL_Point getRotationPivot() const noexcept;

    jt::Vector2f m_offsetFromOrigin { 0.0f, 0.0f };
    jt::Vector2f m_scale { 1.0f, 1.0f };
};

} // namespace jt
