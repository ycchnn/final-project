#include "Bullet.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../shapes/Circle.h"
#include "../shapes/Point.h"
#include <algorithm>
#include <allegro5/bitmap_draw.h>
#include "../data/GIFCenter.h"
#include "../algif5/algif.h"

/*Bullet::Bullet(const Point &p, const Point &target, const std::string &path, double v, int dmg, double fly_dist) {
	ImageCenter *IC = ImageCenter::get_instance();
	this->fly_dist = fly_dist;
	this->dmg = dmg;
	bitmap = IC->get(path);
	double r = std::min(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap)) * 0.8;
	shape.reset(new Circle{p.x, p.y, r});
	double d = Point::dist(p, target);
	vx = (target.x - p.x) * v / d;
	vy = (target.y - p.y) * v / d;
}*/
Bullet::Bullet(const Point &p, const std::string &path, double v, int dmg, double fly_dist) {
    GIFCenter *GIFC = GIFCenter::get_instance();
	gif = GIFC->get(path);  // 從 GIFCenter 獲取 GIF 動畫
    gif_time = 0;           // 初始化 GIF 動畫時間 
    //ImageCenter *IC = ImageCenter::get_instance();
    this->fly_dist = /*fly_dist*/1500;
    this->dmg = dmg;
    //bitmap = IC->get(path);

    // 設定子彈的碰撞體形狀
    double r = std::min(gif->width,gif->height) * 0.8;
    shape.reset(new Circle{p.x, p.y, r});

    // 固定向右飛行的速度
    vx = v;   // 水平速度向右
    vy = 0;   // 垂直速度為 0
}

/**
 * @brief Update the bullet position by its velocity and fly_dist by its movement per frame.
 * @details We don't detect whether to delete the bullet itself here because deleting a object itself doesn't make any sense.
 */
/*void
Bullet::update() {
	if(fly_dist == 0) return;
	DataCenter *DC = DataCenter::get_instance();
	double dx = vx / DC->FPS;
	double dy = vy / DC->FPS;
	double movement = Point::dist(Point{dx, dy}, Point{0, 0});
	if(fly_dist > movement) {
		shape->update_center_x(shape->center_x() + dx);
		shape->update_center_y(shape->center_y() + dy);
		fly_dist -= movement;
	} else {
		shape->update_center_x(shape->center_x() + dx * fly_dist / movement);
		shape->update_center_y(shape->center_y() + dy * fly_dist / movement);
		fly_dist = 0;
	}
}*/
void Bullet::update() {
    if (fly_dist == 0) return;

    DataCenter *DC = DataCenter::get_instance();
    double dx = vx / DC->FPS;
    shape->update_center_x(shape->center_x() + dx);
    fly_dist -= std::abs(dx);

    // 檢查是否飛出螢幕範圍 (假設螢幕寬度是 SCREEN_WIDTH)
    if (shape->center_x() > /*SCREEN_WIDTH*/1264) {
        fly_dist = 0;  // 子彈應該被刪除
    }
}


void
Bullet::draw() {
	/*al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);*/
	DataCenter *DC = DataCenter::get_instance();
	ALLEGRO_BITMAP *current_frame = algif_get_bitmap(gif, gif_time);
	if (current_frame) {
		al_draw_bitmap(
			current_frame,
			shape->center_x() - al_get_bitmap_width(current_frame) / 2,
			shape->center_y() - al_get_bitmap_height(current_frame) / 2,
			0);
	}
	// 更新 GIF 動畫時間
	gif_time += 1.0 / DC->FPS;
}
