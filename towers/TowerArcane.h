//向日葵
#ifndef TOWERARCANE_H_INCLUDED
#define TOWERARCANE_H_INCLUDED

#include <random>
#include <chrono>
#include <iostream>
#include "../sun.h"
#include "Tower.h"
#include "Bullet.h"
#include "../shapes/Point.h"
#include "../data/DataCenter.h"

// fixed settings: TowerArcane attributes
class TowerArcane : public Tower
{
public:
	TowerArcane(const Point &p) : Tower(p, attack_range(), 300, TowerType::ARCANE, 140) {}
	/*Bullet *create_bullet(/*Object *target) {
		const Point &p = Point(shape->center_x(), shape->center_y());
		//const Point &t = Point(target->shape->center_x(), target->shape->center_y());
		//return new Bullet(p, t, TowerSetting::tower_bullet_img_path[static_cast<int>(type)], 480, 4, attack_range());
		return new Bullet(p, TowerSetting::tower_bullet_img_path[static_cast<int>(type)], 480, 4, attack_range());
	}*/
	void create_sun()
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    	std::mt19937 gen(seed);
		//std::random_device rd; // 用於生成隨機種子
    	//std::mt19937 gen(rd()); // 生成隨機數引擎
		std::uniform_real_distribution<> dis_vx(-2.0, 2.0);  // 在 -2.0 到 2.0 之間隨機選取
    	double init_vx = dis_vx(gen);
		std::uniform_real_distribution<> dis_stop_height(shape->center_y() + 5, shape->center_y() + 20);  // 停止高度會隨機在塔的高度 + 5 到 +20 之間
    	double stop_height = dis_stop_height(gen);
		Point tower_center = {shape->center_x(),shape->center_y()};
		// 创建一个向上并向右移动的抛物线投射物
        //double init_vx = 2.0;        // 初始水平速度，可以设为负值表示向左
        double init_vy = -5.0;       // 初始向上的速度
        double gravity = 0.2;        // 重力加速度
        //double stop_height = shape->center_y()+5;  // 停止下落的高度为塔的高度

		//std::cout << "init_vx: " << init_vx << ", init_vy: " << init_vy << ", stop_height: " << stop_height << std::endl;
		DataCenter *DC = DataCenter::get_instance();
		DC->suns.emplace_back(new Sun(tower_center, "./assets/gif/sun.gif", init_vx, init_vy, gravity, stop_height));
	}
	const double attack_range() const { return 160; }
	Bullet *create_bullet() override {
        return nullptr;  
    }
};

#endif
