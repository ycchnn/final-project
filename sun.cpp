#include "sun.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "algif5/algif.h"
#include "shapes/Circle.h"
#include "shapes/Point.h"
#include <allegro5/bitmap_draw.h>
#include "data/ImageCenter.h"

Sun::Sun(const Point &p, const std::string &path, double init_vx, double init_vy, double gravity, double stop_height)
: vx(init_vx), vy(init_vy), gravity(gravity), stop_height(stop_height)
{
    GIFCenter *GIFC = GIFCenter::get_instance();
	gif = GIFC->get(path);  // 從 GIFCenter 獲取 GIF 動畫
    gif_time = 0;           // 初始化 GIF 動畫時間 

    // 設定子彈的碰撞體形狀
    double r = std::min(gif->width,gif->height) * 0.8;
    shape.reset(new Circle{p.x, p.y, r});
};

void Sun::update()
{
    // 只有当投射物高于停止高度时才继续移动
    if (shape->center_y() + vy < stop_height) {
        shape->update_center_x(shape->center_x() + vx);
        shape->update_center_y(shape->center_y() + vy);

        // 应用重力加速度，使垂直速度逐渐增加（向下加速）
        vy += gravity;
    } else {
        // 如果到达停止高度，停止移动
        vy = 0;
        vx = 0;
    }
};

void Sun::draw()
{
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
};

Circle Sun::get_region() const {
        return *shape;
    }