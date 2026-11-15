#pragma once

enum class DamageType { Normal, Fire, Heal };

struct Damage {
    float value { 0.0f };
    DamageType type { DamageType::Normal };
};
