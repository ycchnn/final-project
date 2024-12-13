 #include "Hero.h"
 #include "data/DataCenter.h"
 #include "data/GIFCenter.h"
 #include "algif5/algif.h"
 #include "shapes/Rectangle.h"
 #include "data/ImageCenter.h"

//read gif file

void Hero::init(int y)
{
    ImageCenter *IC = ImageCenter::get_instance();
    char buffer[50];
    sprintf(buffer, "assets/image/weeder.png");
    ALLEGRO_BITMAP *bitmap = IC->get(buffer);
	const double &cx = 220;
	const double &cy = y;
	// We set the hit box slightly smaller than the actual bounding box of the image because there are mostly empty spaces near the edge of a image.
	const int &h = al_get_bitmap_width(bitmap) * 0.8;
	const int &w = al_get_bitmap_height(bitmap) * 0.8;
	shape.reset(new Rectangle{
		(cx - w / 2.), (cy - h / 2.),
		(cx - w / 2. + w), (cy - h / 2. + h)
	});
}   

void Hero::draw()
{   //load gif
   ImageCenter *IC = ImageCenter::get_instance();
	char buffer[50];
    sprintf(buffer, "assets/image/weeder.png");
	ALLEGRO_BITMAP *bitmap = IC->get(buffer);
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}

void Hero::update()
{
    if(state == HeroState::GO)
        shape->update_center_x(shape->center_x() + speed);
}



