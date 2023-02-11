#include "player.h"
#include "graphics.h"
#include "game.h"

Player::Player(const Game& mygame)
	: GameObject(mygame)
	{}

void Player::update()
{
	// Move up, down, left or right

	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		pos_x -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		pos_x += speed * graphics::getDeltaTime() / 10.0f;

	}
	if (graphics::getKeyState(graphics::SCANCODE_W))
	{
		pos_y -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_S))
	{
		if (pos_y <=CANVAS_HEIGHT - 80) {
			pos_y += speed * graphics::getDeltaTime() / 10.0f;
		}
	}
	
	
	// Keeps player inside the canvas aka background
	if (pos_x < 0) pos_x = 0;
	if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;
	if (pos_y < 0) pos_y = 0;
	if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;

}


void Player::draw()
{
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime()/10);

	graphics::Brush br;

	// Vehicle graphics
	br.texture = std::string(ASSET_PATH) + "Ship.png";
	br.outline_opacity = 0.0f;
	graphics::setOrientation(0.f);
    graphics:drawRect(pos_x, pos_y, 263, 134, br);
	// reset rotation
	graphics::setOrientation(0.f);

	// Spacecraft lights
	br.texture = "";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f + glow*0.1f;
	br.fill_color[2] = 1.0f;
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 0.5f;
	br.fill_secondary_color[2] = 1.0f;
	br.fill_opacity = 1.0f;
	br.fill_secondary_opacity = 0.0f;
	br.gradient = true;
	graphics::drawDisk(pos_x+90, pos_y+22.5, 10, br);
	graphics::drawDisk(pos_x+70, pos_y +40, 20, br);


// Radius appear, only for Debug Mode
#ifdef GAME_DEBUG
	br.outline_opacity = 0.0f;
	br.texture = "Player's Spacecraft Radius";
	br.fill_color[0] = 0.3f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 0.3f;
	br.fill_opacity = 0.3f;
	br.gradient = false;
	Disk hull = getCollisionHull();
	graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
#endif

}

// draw player for second level
void Player::draw2()
{
	// Vehicle graphics
	graphics::Brush br;

	br.texture = std::string(ASSET_PATH) + "Ship2.png";
	br.outline_opacity = 0.0f;
	graphics::setOrientation(0.f);

	if (spawn) {
		pos_x = 200;
		pos_y = CANVAS_HEIGHT / 2;
		spawn = false;
	}
graphics:drawRect(pos_x, pos_y, 263, 134, br);

	//Draw princess
	graphics::Brush pr;
	pr.texture = std::string(ASSET_PATH) + "PrincessLeft.png";
	pr.outline_opacity = 0.0f;
	graphics::setOrientation(0.f);
    drawRect(CANVAS_WIDTH-150, CANVAS_HEIGHT-80, 100, 55, pr);
}

void Player::init()
{

}

// Radius settings
Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = 70.0f;
	return disk;
}

