#include "Monster.h"
#include "MonsterWolf.h"
#include "MonsterCaveMan.h"
#include "MonsterWolfKnight.h"
#include "MonsterDemonNinja.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Level.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include "../Utils.h"
#include <allegro5/allegro_primitives.h>
#include "../data/GIFCenter.h"
 #include "../algif5/algif.h"

using namespace std;

// fixed settings
enum class Dir {
	ORI, EAT, FALL, NOHEAD, ANGRY, ANGRY_EAT, LOSEPAPER, ASH,
};
namespace MonsterSetting {
	/*revise
	static constexpr char monster_imgs_root_path[static_cast<int>(MonsterType::MONSTERTYPE_MAX)][40] = {
		"./assets/image/monster/Wolf",
		"./assets/image/monster/CaveMan",
		"./assets/image/monster/WolfKnight",
		"./assets/image/monster/DemonNinja"
	};
	static constexpr char dir_path_prefix[][10] = {
		"UP", "DOWN", "LEFT", "RIGHT"
	};
	*/
	static constexpr char gif_root_path[static_cast<int>(MonsterType::MONSTERTYPE_MAX)][40] = {
		"./assets/gif/zombie/normal",
		"./assets/gif/zombie/bucket",
		"./assets/gif/zombie/newspaper",
		"./assets/gif/zombie/flag"
	};
	static constexpr char gif_postfix[][12] = {
		"original", "eat", "fall", "nohead", "angry", "angry_eat", "losepaper", "ash", 
	};
	//revise end
}

/**
 * @brief Create a Monster* instance by the type.
 * @param type the type of a monster.
 * @param path walk path of the monster. The path should be represented in road grid format.
 * @return The curresponding Monster* instance.
 * @see Level::grid_to_region(const Point &grid) const
 */
Monster *Monster::create_monster(MonsterType type, const vector<Point> &path) {
	switch(type) {
		case MonsterType::WOLF: {
			return new MonsterWolf{path};
		}
		case MonsterType::CAVEMAN: {
			return new MonsterCaveMan{path};
		}
		case MonsterType::WOLFKNIGHT: {
			return new MonsterWolfKnight{path};
		}
		case MonsterType::DEMONNIJIA: {
			return new MonsterDemonNinja{path};
		}
		case MonsterType::MONSTERTYPE_MAX: {}
	}
	GAME_ASSERT(false, "monster type error.");
}

/**
 * @brief Given velocity of x and y direction, determine which direction the monster should face.
 */

Monster::Monster(const vector<Point> &path, MonsterType type) {
	DataCenter *DC = DataCenter::get_instance();
	is_eating = false;
	dead = false;
	shape.reset(new Rectangle{0, 0, 0, 0});
	this->type = type;
	dir = Dir::ORI;
	bitmap_img_id = 0;
	bitmap_switch_counter = 0;
	for(const Point &p : path)
		this->path.push(p);
	if(!path.empty()) {
		const Point &grid = this->path.front();
		const Rectangle &region = DC->level->grid_to_region(grid);
		// Temporarily set the bounding box to the center (no area) since we haven't got the hit box of the monster.
		shape.reset(new Rectangle{region.center_x(), region.center_y(), region.center_x(), region.center_y()});
		this->path.pop();
	}
}

/**
 * @details This update function updates the following things in order:
 * @details * Move pose of the current facing direction (bitmap_img_id).
 * @details * Current position (center of the hit box). The position is moved based on the center of the hit box (Rectangle). If the center of this monster reaches the center of the first point of path, the function will proceed to the next point of path.
 * @details * Update the real bounding box by the center of the hit box calculated as above.
 */
void
Monster::update() {

	char buffer[50];
    sprintf(buffer, "%s/%s.gif",
            MonsterSetting::gif_root_path[static_cast<int>(type)],
            MonsterSetting::gif_postfix[static_cast<int>(dir)]); //print to buffer array
    gifPath[static_cast<int>(type)] = std::string(buffer);

	DataCenter *DC = DataCenter::get_instance();
	//revise
	//ImageCenter *IC = ImageCenter::get_instance();
	if (is_hit) {
        hit_timer -= 1.0 / DC->FPS; // 減少受擊效果時間
        if (hit_timer <= 0) {
            is_hit = false;        // 結束受擊狀態
            brightness = 1.0;      // 恢復正常亮度
        }
    }

	// After a period, the bitmap for this monster should switch from (i)-th image to (i+1)-th image to represent animation.
	if(bitmap_switch_counter) --bitmap_switch_counter;
	else {
		bitmap_img_id = (bitmap_img_id + 1) % (bitmap_img_ids[static_cast<int>(dir)].size());
		bitmap_switch_counter = bitmap_switch_freq;
	}
	
	GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[static_cast<int>(type)]);
    if (gif) {
        // 獲取當前幀的持續時間（以毫秒為單位）
        int frame_duration = gif->frames[current_frame].duration;

        // 遞增幀計時器
        frame_timer += 100 / DataCenter::get_instance()->FPS; // 每幀時間（毫秒）

        // 如果超過幀持續時間，切換到下一幀
        if (frame_timer >= frame_duration) {
            frame_timer = 0;
            current_frame = (current_frame + 1) % gif->frames_count;
        }
    }

	if(type== MonsterType::CAVEMAN){
		if(HP<90){
			type = MonsterType::WOLF;
			char buffer[50];
			sprintf(buffer, "%s/%s.gif",
					MonsterSetting::gif_root_path[static_cast<int>(type)],
					MonsterSetting::gif_postfix[static_cast<int>(dir)]);
			gifPath[static_cast<int>(type)] = std::string(buffer);
		}
	}
	else if(type == MonsterType::WOLFKNIGHT){
		if(HP<90 && HP>0){
			v = 35;
			if(dir == Dir::ORI){
				char buffer[50];
				dir = Dir:: ANGRY;
				sprintf(buffer, "%s/%s.gif",
						MonsterSetting::gif_root_path[static_cast<int>(type)],
						MonsterSetting::gif_postfix[static_cast<int>(dir)]);
				gifPath[static_cast<int>(type)] = std::string(buffer);
			}
			else if(dir == Dir::EAT){
				char buffer[50];
				dir = Dir:: ANGRY_EAT;
				sprintf(buffer, "%s/%s.gif",
						MonsterSetting::gif_root_path[static_cast<int>(type)],
						MonsterSetting::gif_postfix[static_cast<int>(dir)]);
				gifPath[static_cast<int>(type)] = std::string(buffer);
			}
		}
	}

		
	if (is_eating || dead) {
        return; // 怪物暫停，不前進
	}
	
	// v (velocity) divided by FPS is the actual moving pixels per frame.
	double movement = v / (DC->FPS);
	// Keep trying to move to next destination in "path" while "path" is not empty and we can still move.
	while(!path.empty() && movement > 0) {

		const Point &grid = this->path.front();
		const Rectangle &region = DC->level->grid_to_region(grid);
		const Point &next_goal = Point{region.center_x(), region.center_y()};

		// Extract the next destination as "next_goal". If we want to reach next_goal, we need to move "d" pixels.
		double d = Point::dist(Point{shape->center_x(), shape->center_y()}, next_goal);
		
		if(d < movement) {
			// If we can move more than "d" pixels in this frame, we can directly move onto next_goal and reduce "movement" by "d".
			movement -= d;
			shape.reset(new Rectangle{
				next_goal.x, next_goal.y,
				next_goal.x, next_goal.y
			});
			path.pop();
		} else {
			// Otherwise, we move exactly "movement" pixels.
			double dx = (next_goal.x - shape->center_x()) / d * movement;
			double dy = (next_goal.y - shape->center_y()) / d * movement;
			shape->update_center_x(shape->center_x() + dx);
			shape->update_center_y(shape->center_y() + dy);
			movement = 0;
		}
	}
}


void
Monster::draw() {
	GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[static_cast<int>(type)]);
	if (!gif) {
        debug_log("GIF not found: %s\n", gifPath[static_cast<int>(type)].c_str());
        return;
    }
	// 獲取當前幀位圖
    ALLEGRO_BITMAP *frame_bitmap = algif_get_frame_bitmap(gif, current_frame);
    if (!frame_bitmap) {
        debug_log("Frame not found for GIF: %s\n", gifPath[static_cast<int>(type)].c_str());
        return;
    }
	if (is_hit) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // 增加亮度
    }

    // 繪製當前幀
    al_draw_bitmap(
        frame_bitmap,
        shape->center_x() - gif->width / 2,
        shape->center_y() - gif->height / 2,
        0);

    // 恢復默認混合模式
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    //draw gif
    /*algif_draw_gif(gif,
                    shape->center_x() - gif->width/2,
                    shape->center_y() - gif->height/2,
                    0);    */   
	//revise end
}

void Monster::eating() {
	/*
	if (is_eating == false){
		if(dir==Dir::ANGRY){
			dir = Dir::ANGRY_EAT;
		}
		else{
			dir = Dir::EAT;
		}
		char buffer[50];
		sprintf(buffer, "%s/%s.gif",
				MonsterSetting::gif_root_path[static_cast<int>(type)],
				MonsterSetting::gif_postfix[static_cast<int>(dir)]);
		gifPath[static_cast<int>(type)] = std::string(buffer);
	}
	is_eating = true;
	*/
	is_eating = true;
	dir = Dir::EAT;
}

void Monster::resume() {
    is_eating = false;
	if(dir==Dir::ANGRY_EAT){
		dir = Dir::ANGRY;
	}
	else{
		dir = Dir::ORI;
	}
	char buffer[50];
	sprintf(buffer, "%s/%s.gif",
			MonsterSetting::gif_root_path[static_cast<int>(type)],
			MonsterSetting::gif_postfix[static_cast<int>(dir)]);
	gifPath[static_cast<int>(type)] = std::string(buffer);
}

void Monster::die(int x) {
		if(!dead){
			if(x)
				dir = Dir::FALL;
			else
				dir = Dir::ASH;
		char buffer[50];
		sprintf(buffer, "%s/%s.gif",
			MonsterSetting::gif_root_path[static_cast<int>(type)],
			MonsterSetting::gif_postfix[static_cast<int>(dir)]);
			gifPath[static_cast<int>(type)] = std::string(buffer);
		}
		dead = true;
}

Rectangle
Monster::get_region() const {
	return {
		shape->center_x(),
		shape->center_y() + 30,
		shape->center_x() + 90/2,
		shape->center_y() + 144/2
	};
}

