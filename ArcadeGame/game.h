#pragma once

#include "player.h"
#include "enemy.h"

class Game
{
	typedef enum {STATUS_START, STATUS_PLAYING, STATUS_PLAYING2,STATUS_GAMEOVER,STATUS_INTRO} status_t;
	Player *player = nullptr;
	bool player_initializied = false;
	Enemy *meteorite = nullptr;
	bool shot_initialized = false;
	void CrashVocals();
	void takeShot();
	void CaptureVocals();
	void spawnMeteorite();
	void checkMeteorite();
	bool checkCollision();
	bool PortalCollision();
	status_t status = STATUS_START;
	int ro;
	int energy=0;
	int page = 1;
	bool playMusic = false;
	int delay1 = 0;
	float fuel = 1.0f;

	unsigned int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;

	void updateGameOverScreen();
	void updateStartScreen();
	void updateIntroScreen();
	void updateLevelScreen();
	void drawGameOverScreen();
	void drawStartScreen();
	void drawLevelScreen();
	void drawIntroScreen();

public:
	void update();
	void draw();
	void init();
	unsigned int getWindowWidth() { return window_width; }
	unsigned int getWindowHeight() { return window_height; }
	float window2canvasX(float x);
	float window2canvasY(float y);
	int Points;
	float zoom = 0;
	float playTime = 4000;
	float time = 0;
	bool timeInitialized = false;
	bool soundPlayed = false;
	int next = 001;
	int move = CANVAS_WIDTH + 100;
	float rotation;
	void setWindowDimensions(unsigned int w, unsigned int h) { window_width = w; window_height = h; }
	Game(); // Constructor
	~Game();
};