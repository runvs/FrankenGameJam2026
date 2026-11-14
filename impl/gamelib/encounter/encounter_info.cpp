#include "encounter_info.hpp"
#include <fstream>
#include <stdexcept>

void from_json(nlohmann::json const& j, EnemyBaseInfo& v)
{
    j.at("spritesheet").get_to(v.spritesheet);
    j.at("hitPoints").get_to(v.hitPoints);
    j.at("hitPointsMax").get_to(v.hitPointsMax);
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
