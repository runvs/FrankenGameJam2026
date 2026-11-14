#pragma once

#include <color/color.hpp>
#include <color/palette.hpp>
#include <vector.hpp>
#include <cstdint>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName();

    static std::string AuthorName();

    static std::string JamName();

    static std::string JamDate();

    static std::string ExplanationText();

    static jt::Vector2f GetWindowSize();

    static float GetZoom();

    static jt::Vector2f GetScreenSize();

    static jt::Color PaletteBackground();

    static jt::Color PaletteFontFront();

    static jt::Color PalleteFrontHighlight();

    static jt::Color PaletteFontShadow();

    static jt::Color PaletteFontCredits();

    static jt::Palette getPalette();

    static int PhysicSubSteps();
    static jt::Vector2f PlayerSize();

    static int LevelZLayerBelowPlayer();
    static int LevelZLayerPlayer();
    static int LevelZLayerAbovePlayer();
    static int LevelZLayerAboveAll();

    static std::vector<int> LevelZLayerAllLayers();

    static std::uint64_t CollisionBitLevel();
    static std::uint64_t CollisionBitPlayer();
    static std::uint64_t CollisionBitEnemy();
};
