#ifndef SUN_H_INCLUDED
#define SUN_H_INCLUDED
#include <string>
#include <map>
#include "Object.h"
#include "algif5/algif.h"
#include "shapes/Circle.h"

class Sun : public Object
{
public :
    Sun(const Point &p, const std::string &path, double init_vx, double init_vy, double gravity, double stop_height);
    void update();
    void draw();
    Circle get_region() const;
private :
    ALGIF_ANIMATION *gif;  // 使用 ALGIF_ANIMATION 代替 ALLEGRO_BITMAP
    double gif_time;       // 追踪 GIF 動畫播放的時間
    double vx;                        
    double vy;                       
    double gravity;                   // 重力加速度
    double stop_height;               // 停止下落的高度
    int width, height;
    std::shared_ptr<Circle> shape;  // 形狀，這裡使用 shared_ptr 管理 Circle 物件
};


#endif