#include "enemy.h"
#include "graphics.h"
#include "config.h"
#include <random>
#include "util.h"
#include "game.h"

void Enemy::update()
{
	// Asteroid or Crystal moving directions 
	pos_x -= speed * graphics::getDeltaTime();
	rotation += 0.1f*graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);

	// Check if asteroid is on screen, if it isnt then, "active = false"
	if (pos_x < -size)
	{
		active = false;
	}
} 


void Enemy::draw()
{

	// Set rotation
	graphics::setOrientation(rotation);

	if (type == 1) {
		brush.texture = std::string(ASSET_PATH) + "Asteroid.png";
		graphics::drawRect(pos_x, pos_y, size, size, brush);
	}
	else if (type == 2) {
		brush.texture = std::string(ASSET_PATH) + "CrystalLife.png";
		graphics::drawRect(pos_x, pos_y,50, 90, brush);
		
		// Create glow for green crystal
		float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);

		graphics::Brush br;
		br.texture = "";
		br.fill_color[0] = 0.0f;
		br.fill_color[1] = 1.0f + glow * 0.4f;
		br.fill_color[2] = 0.0f;
		br.fill_secondary_color[0] = 0.0f;
		br.fill_secondary_color[1] = 1.0f;
		br.fill_secondary_color[2] = 0.8f;
		br.fill_opacity = 1.0f;
		br.fill_secondary_opacity = 0.0f;
		br.gradient = true;
		br.outline_opacity = 0;
		graphics::drawDisk(pos_x , pos_y , 30, br);

	}
	else if (type == 3) {
		graphics::resetPose();
		brush.texture = std::string(ASSET_PATH) + "shot.png";
		graphics::drawRect(pos_x, pos_y, 50, 8, brush);

		// Create glow for shot
		float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);

		graphics::Brush br;
		br.texture = "";
		br.fill_color[0] = 1.0f + glow * 0.4f;
		br.fill_color[1] = 0.94f;
		br.fill_color[2] = 0.11f;
		br.fill_secondary_color[0] = 0.0f;
		br.fill_secondary_color[1] = 1.0f;
		br.fill_secondary_color[2] = 0.8f;
		br.fill_opacity = 1.0f;
		br.fill_secondary_opacity = 0.0f;
		br.gradient = true;
		br.outline_opacity = 0;
		graphics::drawDisk(pos_x - 30, pos_y, 20, br);
	}
	else if (type == 4) {
		brush.texture = std::string(ASSET_PATH) + "CrystalEnergy.png";
		graphics::drawRect(pos_x, pos_y, 80, 90, brush);

		// Create glow for purple crystal
		float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);

		graphics::Brush br;
		br.texture = "";
		br.fill_color[0] = 1.0f + glow * 0.4f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 1.0f;
		br.fill_opacity = 0.0f;
		br.fill_secondary_opacity = 0.f;
		br.gradient = true;
		br.outline_opacity = 0;
		graphics::drawDisk(pos_x, pos_y, 30, br);
	}

	brush.fill_opacity = 1.0;
	// Reset rotation
	graphics::resetPose();

// Radius appear, only for Debug Mode
#ifdef GAME_DEBUG
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.texture = "Asteroid radius";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.3f;
	br.fill_color[2] = 0.3f;
	br.fill_opacity = 0.3f;
	br.gradient = false;
	Disk hull = getCollisionHull();
	graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
#endif



}

void Enemy::init()
{  
// Speed and spawn values of objects
	speed = 1.3f;
	pos_x = CANVAS_WIDTH + 1.1f * size;
	pos_y = CANVAS_HEIGHT * rand0to1();
	size = 50 + 100 * rand0to1();
	rotation = 360 * rand0to1();
	brush.outline_opacity = 0.0f;
}

// Returns Type of Object
int Enemy::getType()
{
	return type;
}

// Upgrades Speed
void Enemy::speedUp()
{
	speed = 1.6f;
}


Enemy::Enemy(const Game& mygame)
	: GameObject(mygame)
{
	ro = rand() % 30 + 1;
	if (ro <= 18) {
		type = 1; // asteroid
	}
	else if (ro <= 20) {
		type = 2; // life crystal
	}
	else if (ro <= 28) {
		type = 3; // shot
	}
	else if (ro <= 30) {
		type = 4; // Energy Crystal
	}
	init();
}

Enemy::~Enemy()
{
}

// Radius settings
Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size - size/2;
	return disk;
}
