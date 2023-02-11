#pragma once
#include "gameobject.h"
#include "graphics.h"
#include <iostream>;

using namespace std;


class Enemy : public GameObject
{
	int type;
	int ro;
	float pos_x, pos_y;
	float speed;
	float size;
	float rotation;
	graphics::Brush brush;
	bool active = true;

public:
	void speedUp();
	void draw() override;
	void update() override;
	void init() override;
	int getType(); // Returns type of object
	bool isActive() { return active; }
	float getSize() { return size; }
	Enemy(const class Game & mygame);
	~Enemy();
	Disk getCollisionHull() const;
};