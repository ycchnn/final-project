//土豆雷
#ifndef TOWERPOISON_H_INCLUDED
#define TOWERPOISON_H_INCLUDED

#include "Tower.h"
#include "Bullet.h"
#include "../shapes/Point.h"

// fixed settings: TowerPoison attributes
class TowerPoison : public Tower
{
public:
	TowerPoison(const Point &p) : Tower(p, attack_range(), 30, TowerType::POISON, 140) {}
	/*Bullet *create_bullet(/*Object *target) {
		const Point &p = Point(shape->center_x(), shape->center_y());
		//const Point &t = Point(target->shape->center_x(), target->shape->center_y());
		return new Bullet(p, TowerSetting::tower_bullet_img_path[static_cast<int>(type)], 480, 6, attack_range());
		//return new Bullet(p, t, TowerSetting::tower_bullet_img_path[static_cast<int>(type)], 480, 6, attack_range());
	}*/
	const double attack_range() const { return 150; }
	void update() override {};
	Bullet *create_bullet() override {
        return nullptr;  
    }
};

#endif
