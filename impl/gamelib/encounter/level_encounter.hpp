#pragma once
#include <game_object.hpp>

class LevelEncounter : public jt::GameObject {
public:

private:
    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;
};
