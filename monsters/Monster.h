#ifndef MONSTER_H_INCLUDED
#define MONSTER_H_INCLUDED

#include "../Object.h"
#include "../shapes/Rectangle.h"
#include <vector>
#include <queue>
#include <map>

enum class Dir;

// fixed settings
enum class MonsterType {
	WOLF, CAVEMAN, WOLFKNIGHT, DEMONNIJIA, MONSTERTYPE_MAX
};

/**
 * @brief The class of a monster (enemies).
 * @details Monster inherits Object and takes Rectangle as its hit box.
 */
class Monster : public Object
{
public:
	static Monster *create_monster(MonsterType type, const std::vector<Point> &path);
public:
	Monster(const std::vector<Point> &path, MonsterType type);
	void update();
	void draw();
	void eating();
	void resume();
	int current_frame = 0;     // 當前幀索引
    int frame_timer = 0;       // 計算幀時間
	float death_timer = 1.4f; // -1 表示未死亡状态
    void die();
    bool is_dead() const {
        return death_timer <= 0.0f;
    }
    void update_death_timer() {
		float delta_time = 0.016f;
        if (death_timer >= 0) {
            death_timer -= delta_time;
        }
    }
	const int &get_money() const { return money; }
	int HP;
	const std::queue<Point> &get_path() const { return path; }
	bool is_hit = false;
	float hit_timer = 0;
	float brightness = 1;
	Dir dir;
	bool dead;
protected:
	/**
	 * @var HP
	 * @brief Health point of a monster.
	 **
	 * @var v
	 * @brief Moving speed of a monster.
	 **
	 * @var money
	 * @brief The amount of money that player will earn when the monster is killed.
	 **
	 * @var bitmap_img_ids
	 * @brief The first vector is the Dir index, and the second vector is image id.
	 * @details `bitmap_img_ids[Dir][<ordered_id>]`
	 **
	 * @var bitmap_switch_counter
	 * @brief Counting down for bitmap_switch_freq.
	 * @see Monster::bitmap_switch_freq
	 **
	 * @var bitmap_switch_freq
	 * @brief Number of frames required to change to the next move pose for the current facing direction.
	 * @details The variable is left for child classes to define.
	 * 
	 * @var bitmap_img_id
	 * @brief Move pose of the current facing direction.
	 **
	 * @var dir
	 * @brief Current facing direction.
	 **
	 * @var path
	 * @brief The walk path of a monster, represented in grid format.
	 * @see Level::grid_to_region(const Point &grid) const
	*/
	int v;
	int money;
	std::vector<std::vector<int>> bitmap_img_ids;
	int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id;
	//revise start
	std::map<int, std::string> gifPath; // 每種怪物類型對應的 GIF 路徑
	bool is_eating; 
	//revise end
private:
	MonsterType type;
	//Dir dir;
	std::queue<Point> path;
};

#endif
