#include "crystal.h"
#include "graphics.h"
#include "config.h"
#include <random>
#include "util.h"
#include "game.h"

void Crystal::update()
{
	// Crystal moving directions 
	pos_x -= speed * graphics::getDeltaTime();
	rotation += 0.1f * graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);

	// Check if crystal is on screen, if it isnt then, "active = false"
	if (pos_x < -size)
	{
		active = false;
	}
}


void Crystal::draw()
{
	// Set rotation
	graphics::setOrientation(rotation);

	brush.texture = std::string(ASSET_PATH) + "crystal.png";
	brush.fill_opacity = 1.0;
	graphics::drawRect(pos_x, pos_y, size, size, brush);
	// Reset rotation
	graphics::resetPose();

	// Radius appear, only for Debug Mode
#ifdef GAME_DEBUG
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.texture = "";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.3f;
	br.fill_color[2] = 0.3f;
	br.fill_opacity = 0.3f;
	br.gradient = false;
	Disk hull = getCollisionHull();
	graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
#endif

}

void Crystal::init()
{
	// Speed and spawn values of asteroid
	speed = 1.3f;
	pos_x = CANVAS_WIDTH + 1.1f * size;
	pos_y = CANVAS_HEIGHT * rand0to1();
	size = 50 + 100 * rand0to1();
	rotation = 360 * rand0to1();
	brush.outline_opacity = 0.0f;
}



Crystal::Crystal(const Game& mygame)
	: GameObject(mygame)
{
	init();
}

Crystal::~Crystal()
{
}

// Radius settings
Disk Crystal::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size - size / 2;
	return disk;
}
