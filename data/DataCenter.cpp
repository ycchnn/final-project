#include "DataCenter.h"
#include <cstring>
#include "../Level.h"
#include "../Player.h"
#include "../monsters/Monster.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
//revise start
#include "../Hero.h"
//revise end

// fixed settings
namespace DataSetting {
	constexpr double FPS = 60;
	constexpr int window_width = 1264;
	constexpr int window_height = 628;
	constexpr int game_field_length = 1100;
}

DataCenter::DataCenter() {
	this->FPS = DataSetting::FPS;
	this->window_width = DataSetting::window_width;
	this->window_height = DataSetting::window_height;
	this->game_field_length = DataSetting::game_field_length;
	memset(key_state, false, sizeof(key_state));
	memset(prev_key_state, false, sizeof(prev_key_state));
	mouse = Point(0, 0);
	memset(mouse_state, false, sizeof(mouse_state));
	memset(prev_mouse_state, false, sizeof(prev_mouse_state));
	player = new Player();
	level = new Level();
	//revise start
	for(int i = 0; i<5; i++){
		heros.emplace_back(new Hero());
	}
	//revise end
}

DataCenter::~DataCenter() {
	delete player;
	delete level;
	for(Monster *&m : monsters) {
		delete m;
	}
	for(Tower *&t : towers) {
		delete t;
	}
	for(Bullet *&b : towerBullets) {
		delete b;
	}
	for(Hero *&h : heros) {
		delete h;
	}
}

void DataCenter::reset(){
	delete player;
	player = new Player();

    // 清理关卡数据
    delete level;
    level = new Level();

    // 清空怪物列表
    for (Monster* monster : monsters) {
        delete monster;
    }
    monsters.clear();

    // 重置英雄数据
    for (int i = 0; i < 5; ++i) {
        delete heros[i];
        heros[i] = new Hero();
    }

    // 其他资源的重置逻辑...
	 for (Tower* tower : towers) {
        delete tower;
    }
    towers.clear();
}
