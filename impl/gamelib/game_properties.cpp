#include "game_properties.hpp"
#include <color/palette_builder.hpp>
#include <box2d/box2d.h>

namespace {

jt::Palette createPalette()
{
    jt::PaletteBuilder builder;
    return builder
        .addColorsFromGPL(R"(GIMP Palette
#Palette Name: Dreamscape8
#Description: A palette made of low saturation colours to give your art a dreamlike quality.
#Colors: 8
201	204	161	c9cca1
202	160	90	caa05a
174	106	71	ae6a47
139	64	73	8b4049
84	51	68	543344
81	82	98	515262
99	120	125	63787d
142	160	145	8ea091
)")
        .create();
}

} // namespace

std::string GP::GameName() { return "MyAwesomeGame"; }

std::string GP::AuthorName() { return "TODO"; }

std::string GP::JamName() { return "TODO"; }

std::string GP::JamDate() { return "TODO"; }

std::string GP::ExplanationText()
{
    return "Win the game\n[W,A,S,D] to move \n[Space] to jump\n[M/U] to mute/unmute audio";
}

jt::Vector2f GP::GetWindowSize() { return jt::Vector2f { 1280, 960 }; }

float GP::GetZoom() { return 4.0f; }

jt::Vector2f GP::GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

jt::Color GP::PaletteBackground() { return GP::getPalette().getColor(4); }

jt::Color GP::PaletteFontFront() { return GP::getPalette().getColor(0); }

jt::Color GP::PalleteFrontHighlight() { return GP::getPalette().getColor(1); }

jt::Color GP::PaletteFontShadow() { return GP::getPalette().getColor(2); }

jt::Color GP::PaletteFontCredits() { return GP::getPalette().getColor(1); }

jt::Palette GP::getPalette()
{
    static auto const p = createPalette();
    return p;
}

int GP::PhysicSubSteps() { return 4; }

jt::Vector2f GP::PlayerSize() { return jt::Vector2f { 16.0f, 16.0f }; }

jt::Vector2f GP::LevelTileSize() { return jt::Vector2f { 24.0f, 24.0f }; }

int GP::LevelZLayerBelowPlayer() { return 0; }

int GP::LevelZLayerPlayer() { return 1; }

int GP::LevelZLayerAbovePlayer() { return 3; }

int GP::LevelZLayerAboveAll() { return 4; }

std::vector<int> GP::LevelZLayerAllLayers()
{
    return { GP::LevelZLayerBelowPlayer(), GP::LevelZLayerPlayer(), GP::LevelZLayerAbovePlayer(),
        GP::LevelZLayerAboveAll() };
}

std::uint64_t GP::CollisionBitLevel() { return B2_DEFAULT_CATEGORY_BITS; }

std::uint64_t GP::CollisionBitPlayer() { return 1 << 1; }

std::uint64_t GP::CollisionBitEnemy() { return 1 << 2; }

float GP::PlayerBaseVelocity() { return 85.0f; }

float GP::CameraSpeed() { return 100.0f; }

jt::Color GP::BattleMenuBackgroundColor() { return jt::Color { 45, 47, 44 }; }

jt::Color GP::BattleMenuSelectorColor() { return jt::Color { 175, 178, 166 }; }

jt::Color GP::BattleMenuStateBaseColor() { return jt::Color { 255, 255, 255, 255 }; }

jt::Color GP::BattleMenuStateSelectedColor() { return jt::Color { 255, 100, 100, 255 }; }

float GP::BattleMenuTweenTime() { return 0.2f; }

float GP::BattleTimeBetweenActors() { return 0.7f; }

float GP::CameraMarginTileScaleValue() { return 3.0f; }
