#pragma once
#include <nlohmann.hpp>
#include <string>
#include <vector>

struct EnemyBaseInfo {
    std::string spritesheet;
    float hitPoints { 0.0f };
    float hitPointsMax { 0.0f };
    float Attack { 0.0f };
    float Defense { 0.0f };
    // TODO more values
};

struct EnemyEncounterInfo {
    EnemyBaseInfo enemyBase;
    int level { 0 };
    int position { 0 };
};

struct EncounterInfo {
    std::vector<EnemyEncounterInfo> enemies;
    std::string levelSprite;
    std::string backgroundSprite;
};

void from_json(nlohmann::json const& j, EnemyBaseInfo& v);

void from_json(nlohmann::json const& j, EnemyEncounterInfo& v);

void from_json(nlohmann::json const& j, EncounterInfo& v);

nlohmann::json loadJsonFromFile(std::string const& fileName);
EnemyBaseInfo loadEnemyBaseInfo(std::string const& fileName);
EncounterInfo loadEncounterInfo(std::string const& fileName);
