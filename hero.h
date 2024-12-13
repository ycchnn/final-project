#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include <string>
#include <map>
#include "Object.h"

enum class HeroState
{
    STOP,
    GO
};

class Hero : public Object
{
public:
    void init(int y);
    void update();
    void draw();
    HeroState state = HeroState::STOP;
private:
    double speed = 5;
};

#endif