#include "Tower.h"
#include "TowerArcane.h"
#include "TowerArcher.h"
#include "TowerCanon.h"
#include "TowerPoison.h"
#include "TowerStorm.h"
#include "../Utils.h"
#include "../shapes/Circle.h"
#include "../monsters/Monster.h"
#include "../shapes/Rectangle.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../data/SoundCenter.h"
#include <allegro5/bitmap_draw.h>
#include "../data/GIFCenter.h"
#include "../algif5/algif.h"

// fixed settings
namespace TowerSetting {
	constexpr char attack_sound_path[] = "./assets/sound/Arrow.wav";
};

/*revise
ALLEGRO_BITMAP*
Tower::get_bitmap(TowerType type) {
	ImageCenter *IC = ImageCenter::get_instance();
	return IC->get(TowerSetting::tower_full_img_path[static_cast<int>(type)]);
}
*/
ALGIF_ANIMATION *Tower::get_animation(TowerType type) {
    GIFCenter *GIFC = GIFCenter::get_instance(); 
    const std::string &path = TowerSetting::tower_gif_path[static_cast<int>(type)];
    return GIFC->get(path); 
}
//revise end

Tower*
Tower::create_tower(TowerType type, const Point &p) {
	switch(type) {
		case TowerType::ARCANE: {
			return new TowerArcane(p);
		} case TowerType::ARCHER: {
			return new TowerArcher(p);
		} case TowerType::CANON: {
			return new TowerCanon(p);
		} case TowerType::POISON: {
			return new TowerPoison(p);
		} case TowerType::STORM: {
			return new TowerStorm(p);
		} case TowerType::TOWERTYPE_MAX: {}
	}
	GAME_ASSERT(false, "tower type error.");
}

/**
 * @param p center point (x, y).
 * @param attack_range any monster inside this number would trigger attack.
 * @param attack_freq period for tower to attack.
 * @param type tower type.
*/
Tower::Tower(const Point &p, double attack_range, int attack_freq, TowerType type, int h) {
	//revise start
	//ImageCenter *IC = ImageCenter::get_instance();
	GIFCenter *GIFC = GIFCenter::get_instance();
	//revise end
	// shape here is used to represent the tower's defending region. If any monster walks into this area (i.e. the bounding box of the monster and defending region of the tower has overlap), the tower should attack.
	shape.reset(new Circle(p.x, p.y, attack_range));
	counter = 0;
	this->attack_freq = attack_freq;
	this->type = type;
	//revise
	animation = GIFC->get(TowerSetting::tower_gif_path[static_cast<int>(type)]);
	hp = h;
	planted = false;
}

/**
 * @brief Update attack cooldown and detect if the tower could make an attack.
 * @see Tower::attack(Object *target)
*/
void
Tower::update() {
	TowerArcane* sunTower = dynamic_cast<TowerArcane*>(this);
	if(counter) counter--;
	else {
		if(type == TowerType::ARCHER) attack();
		else if(sunTower)
		{
			//std::cout << "Creating sun from TowerArcane" << std::endl; // 打印是否進入該條件
			sunTower->create_sun();
			counter = attack_freq;
		}
	}
}

/**
 * @brief Check whether the tower can attack the target. If so, shoot a bullet to the target.
*/
bool
Tower::attack(/*Object *target*/) {
	if(counter) return false;
	//if(!target->shape->overlap(*shape)) return false;
	DataCenter *DC = DataCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	DC->towerBullets.emplace_back(create_bullet());
	SC->play(TowerSetting::attack_sound_path, ALLEGRO_PLAYMODE_ONCE);
	counter = attack_freq;
	return true;
}

void
Tower::draw() {
	/*revise
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap)/2,
		shape->center_y() - al_get_bitmap_height(bitmap)/2, 0);
	*/
	GIFCenter *GIFC = GIFCenter::get_instance();
    //draw gif
    /*algif_draw_gif(animation,
                    shape->center_x() - animation->width/2,
                    shape->center_y() - animation->height/2,
                    0);       */
	//revise end
	/*if (planted) {
        // 播放動畫
        algif_draw_gif(animation,
                       shape->center_x() - animation->width / 2,
                       shape->center_y() - animation->height / 2,
                       0);
    } else {
        // 定格在第一幀
		debug_log("not planted\n");
        ALLEGRO_BITMAP *first_frame = algif_get_bitmap(animation, 0);
        al_draw_bitmap(first_frame,
                       shape->center_x() - al_get_bitmap_width(first_frame) / 2,
                       shape->center_y() - al_get_bitmap_height(first_frame) / 2,
                       0);
    }*/
	if (!planted) {
        // 预览状态：显示动画的第一帧（定格）
        ALLEGRO_BITMAP *first_frame = algif_get_frame_bitmap(animation, 0);
        if (first_frame) {
            al_draw_bitmap(first_frame, shape->center_x() - al_get_bitmap_width(first_frame) / 2,
                       shape->center_y() - al_get_bitmap_height(first_frame) / 2,
                       0);
        }
    } else {
        // 已放置状态：播放完整动画
        algif_draw_gif(animation,
                       shape->center_x() - animation->width / 2,
                       shape->center_y() - animation->height / 2,
                       0);
    }
}

/**
 * @brief Get the area of the tower, and return with a Rectangle object.
*/
Rectangle
Tower::get_region() const {
	//revise start
	//int w = al_get_bitmap_width(bitmap);
	//int h = al_get_bitmap_height(bitmap);
	int w = animation->width;
	int h = animation->height;
	//revise end
	if(type == TowerType::POISON)
	{
		return {
			shape->center_x() - 80/2,
			shape->center_y() - (150-55/2),
			shape->center_x() + 80/2,
			shape->center_y() + 55/2
		};
	}
	else return {
		shape->center_x() - w/2,
		shape->center_y() - h/2,
		shape->center_x() - w/2 + w,
		shape->center_y() - h/2 + h
	};
}
//test test