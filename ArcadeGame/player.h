#pragma once
#include "gameobject.h"
#include "config.h"

class Player : public GameObject, public Collidable
{
	float speed = 2.0f;
	float pos_x = CANVAS_WIDTH/2, pos_y=CANVAS_HEIGHT/2;
	float life = 1.0f;
public:
	Player(const class Game& mygame);
	void update() override;
	void draw() override;
	void draw2();
	void init() override;
	float getPosX() { return pos_x;}
	float getPosY() {return pos_y;}
	Disk getCollisionHull() const override;
	float getRemainingLife() const { return life; }
	void setRemainingLife(float newlife) { life = newlife; }
	bool spawn = true;

	// Decrase life in every crash
	void drainLife(float amount) { life = std::max<float>(0.0f, life-amount);}
	// Gives Life
	void gainLife() {
		if (life + 0.1f <= 1.0f) {
			life = std::max<float>(0.0f, life + 0.1f);
		}
		else {
			life = 1.0f;
		}
	}

};