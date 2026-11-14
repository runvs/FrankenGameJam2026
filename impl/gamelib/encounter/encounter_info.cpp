#include "encounter_info.hpp"
#include <fstream>
#include <stdexcept>

EnemyBaseInfo getLeveled(EnemyEncounterInfo const& enemyEncounterInfo)
{
    EnemyBaseInfo info = enemyEncounterInfo.enemyBase;

    // TODO get leveled up stats

    info.hitPointsMax = (1.0f + 0.2f * (enemyEncounterInfo.level - 1))
        * enemyEncounterInfo.enemyBase.hitPointsMax;
    info.hitPoints
        = (1.0f + 0.2f * (enemyEncounterInfo.level - 1)) * enemyEncounterInfo.enemyBase.hitPoints;

    info.attack
        = (1.0f + 0.1f * (enemyEncounterInfo.level - 1)) * enemyEncounterInfo.enemyBase.attack;

    return info;
}

void from_json(nlohmann::json const& j, EnemyBaseInfo& v)
{
    j.at("spritesheet").get_to(v.spritesheet);
    j.at("hitPoints").get_to(v.hitPoints);
    j.at("hitPointsMax").get_to(v.hitPointsMax);
    j.at("attack").get_to(v.attack);
    j.at("defense").get_to(v.defense);
}

void from_json(nlohmann::json const& j, EnemyEncounterInfo& v)
{
    auto const& enemyBase = j.at("enemyBase");
    if (enemyBase.is_string()) {
        v.enemyBase = loadEnemyBaseInfo(enemyBase.get<std::string>());
    } else {
        enemyBase.get_to(v.enemyBase);
    }
    j.at("level").get_to(v.level);
    j.at("position").get_to(v.position);
}

void from_json(nlohmann::json const& j, EncounterInfo& v)
{
    j.at("enemies").get_to(v.enemies);
    j.at("level_sprite").get_to(v.levelSprite);
    j.at("background_sprite").get_to(v.backgroundSprite);
}

nlohmann::json loadJsonFromFile(std::string const& fileName)
{
    std::ifstream file { fileName };
    if (!file.is_open()) {
        throw std::invalid_argument { "Could not open json file: " + fileName };
    }
    return nlohmann::json::parse(file);
}

EnemyBaseInfo loadEnemyBaseInfo(std::string const& fileName)
{
    return loadJsonFromFile(fileName).get<EnemyBaseInfo>();
}

EncounterInfo loadEncounterInfo(std::string const& fileName)
{
    return loadJsonFromFile(fileName).get<EncounterInfo>();
}
