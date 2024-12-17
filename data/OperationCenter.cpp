#include "OperationCenter.h"
#include "DataCenter.h"
#include "../monsters/Monster.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
#include "../Player.h"
//revise start
#include "../Hero.h"
#include "../sun.h"
#include <iostream>
//revise end

void OperationCenter::update() {
	// Update monsters.
	_update_monster();
	// Update towers.
	_update_tower();
	// Update tower bullets.
	_update_towerBullet();
	// If any bullet overlaps with any monster, we delete the bullet, reduce the HP of the monster, and delete the monster if necessary.
	_update_monster_towerBullet();
	// If any monster reaches the end, hurt the player and delete the monster.
	_update_monster_player();
	//i2p revise start
	_update_monster_tower();
	_update_monster_hero();
	//revise end
	_update_sun();
	_cherrybomb();
}

void OperationCenter::_update_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->update();
}

void OperationCenter::_update_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->update();
}

void OperationCenter::_update_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->update();
	// Detect if a bullet flies too far (exceeds its fly distance limit), which means the bullet lifecycle has ended.
	for(size_t i = 0; i < towerBullets.size(); ++i) {
		if(towerBullets[i]->get_fly_dist() <= 0) {
			towerBullets.erase(towerBullets.begin()+i);
			--i;
		}
	}
}

void OperationCenter::_update_monster_towerBullet() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Bullet*> &towerBullets = DC->towerBullets;
	for(size_t i = 0; i < monsters.size(); ++i) {
		for(size_t j = 0; j < towerBullets.size(); ++j) {
			// Check if the bullet overlaps with the monster.
			if(monsters[i]->get_region().overlap(*(towerBullets[j]->shape))) {
				monsters[i]->is_hit = true;
				monsters[i]->hit_timer = 0.3;
				monsters[i]->brightness = 1.5;
				// Reduce the HP of the monster. Delete the bullet.
				monsters[i]->HP -= towerBullets[j]->get_dmg();
				towerBullets.erase(towerBullets.begin()+j);
				--j;
				if(monsters[i]->HP <=0 && !monsters[i]->dead){
					monsters[i]->die(1);
					break;
				}
			}
		}
	}
}

//revise

void OperationCenter::_update_monster_tower() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Tower*> &towers = DC->towers;
	Player *&player = DC->player;
	for(size_t i = 0; i < monsters.size(); ++i) {
		for(size_t j = 0; j < towers.size(); ++j) {
			// Check if the plant overlaps with the monster.
			if(monsters[i]->get_region().overlap(towers[j]->get_region())) {
				if(towers[j]->type == TowerType::POISON)
				{
					std::cout << "bomb\n" ;
					monsters[i]->HP = 0;
					if(!monsters[i]->dead){
						monsters[i]->die(0);
					}
                    towers.erase(towers.begin() + j);
                    --j;
                    break;
				}
				else{
					monsters[i]->eating();
					towers[j]->hp -= 1;
					if(towers[j]->hp <= 0) {
						towers.erase(towers.begin()+j);
						--j;
						monsters[i]->resume();
						// Since the current plant is killed, we can directly proceed to next monster.
					}
				}
			}
		}
	}
}

void OperationCenter::_update_monster_player() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	Player *&player = DC->player;
	for(size_t i = 0; i < monsters.size(); ++i) {
		// Check if the monster is killed.
		if(monsters[i]->dead ){
			monsters[i]->update_death_timer();
		}
		if (monsters[i]->is_dead()) {
			//player->coin += monsters[i]->get_money();
            monsters.erase(monsters.begin()+i);
			--i;
            break; 
        }
		// Check if the monster reaches the end.
		if(monsters[i]->get_path().empty()) {
			monsters.erase(monsters.begin()+i);
			player->HP--;
			--i;
		}
	}
}


//i2p revise s
void OperationCenter::_update_monster_hero()
{
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster *> &monsters = DC->monsters;
	for (size_t i = 0; i < monsters.size(); ++i)
	{
		for(int j = 0; j<5; j++){
			if (monsters[i]->shape->overlap(*(DC->heros[j]->shape)))
			{
				monsters[i]->HP = 0;
				if(!monsters[i]->dead)
					monsters[i]->die(1);
				DC->heros[j]->state = HeroState::GO;
			}
		}
	}
}
//i2p revise e

void OperationCenter::_cherrybomb()
{
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster *> &monsters = DC->monsters;
	std::vector<Tower*> &towers = DC->towers;
	for(size_t j = 0; j < towers.size(); ++j){
		if(towers[j]->type == TowerType::STORM){
			bool bombed = false;
			towers[j]->placed_time++;
			for (size_t i = 0; i < monsters.size(); ++i)
			{
				
				if (monsters[i]->shape->overlap(towers[j]->get_attack_range()))
				{
					bombed = true;
					//monsters[i]->HP = 0;
					if(!monsters[i]->dead){
						monsters[i]->die(0);
					}
				}
				
			}
			if(bombed||towers[j]->placed_time>= 2*DC->FPS){
					towers.erase(towers.begin()+j);
					--j;
					break;
			}
		}
		
	}
}


void OperationCenter::_update_sun()
{
	std::vector<Sun *> &suns = DataCenter::get_instance()->suns;
	for(Sun *sun : suns)
		sun->update();
}

void OperationCenter::draw() {
	_draw_monster();
	_draw_tower();
	_draw_towerBullet();
	_draw_sun();
}

void OperationCenter::_draw_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->draw();
}

void OperationCenter::_draw_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->draw();
}

void OperationCenter::_draw_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->draw();
}

void OperationCenter::_draw_sun() {
    std::vector<Sun *> &suns = DataCenter::get_instance()->suns;
	for(Sun *sun : suns)
		sun->draw();
}

