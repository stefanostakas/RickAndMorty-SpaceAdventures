#pragma once
#include "gameobject.h"
#include "graphics.h"

class Crystal : public GameObject
{
	float pos_x, pos_y;
	float speed;
	float size;
	float rotation;
	graphics::Brush brush;
	bool active = true;

public:
	void update() override;
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	float getSize() { return size; }
	Crystal(const class Game& mygame);
	~Crystal();
	Disk getCollisionHull() const;
};