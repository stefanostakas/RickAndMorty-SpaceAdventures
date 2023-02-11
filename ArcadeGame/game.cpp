#include "game.h"
#include "graphics.h"
#include "config.h"
#include <iostream>
#include "player.h"
#include<windows.h>


void Game::CaptureVocals()
{
	if (player->getRemainingLife() > 0.0) {
		int v1 = rand() % 2 + 1;         // Change first int with the total sound clips of the crashvocal folder
		string file = to_string(v1) + "CaptureVocal.wav";
		graphics::playSound(std::string(CAPTURE_VOCALS) + file, 1.0f, false);
	}
}

void Game::CrashVocals()
{
	if (player->getRemainingLife() > 0.0) {
		int v1 = rand() % 10 + 1;         // Change first int with the total sound clips of the crashvocal folder
		string file = to_string(v1) + "crashvocal.mp3";
		graphics::playSound(std::string(CRASH_VOCALS) + file, 0.7f, false);
	}
}



void Game::spawnMeteorite()
{
	if (!meteorite && delay1 > 200) {
		meteorite = new Enemy(*this);

		if (fuel > 0.f) {
			fuel -= 0.02;
		}
		else {
			status =STATUS_GAMEOVER;
		}
        
		if (meteorite->getType()==1 || meteorite->getType()==3) {
			Points += 1;
		}
	}
	delay1++;
}

//If meteorite is not active delete it.
void Game::checkMeteorite()
{
	if (meteorite && !meteorite->isActive()) {
		delete meteorite;
		meteorite = nullptr;
	}
}



bool Game::checkCollision()
{
	if (!player || !meteorite)
	{
		return false;
	}

	Disk d1 = player->getCollisionHull();
	Disk d2 = meteorite->getCollisionHull();
	
	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		if (meteorite->getType() == 1) {

			//If life is equal to zero, GAME OVER
			if (player->getRemainingLife() == 0) {
				status = STATUS_GAMEOVER;
				graphics::playSound(std::string(ASSET_PATH) + "GameOver.mp3", 0.3f, false);
				update();
			}
			// Drain life
			player->drainLife(meteorite->getSize() / 300);
			return true;
		}else if (meteorite->getType() == 2) {
			player->gainLife();
		}
		else if (meteorite->getType() == 3) {
			//If life is equal to zero, GAME OVER
			if (player->getRemainingLife() == 0) {
				status = STATUS_GAMEOVER;
				graphics::playSound(std::string(ASSET_PATH) + "GameOver.mp3", 0.3f, false);
				update();
			}
			player->drainLife(0.1f);
			return true;
		}
	}
	else 
		return false;
	
}


bool Game::PortalCollision()
{
	if (!player || zoom < 150)
	{
		return false;
	}

	Disk d1 = player->getCollisionHull();
	

	if (d1.cx > 1100 && d1.cx < 1300 && d1.cy > 250 && d1.cy < 450 )
	{
		status = STATUS_PLAYING2;
		update();
		return true;
		}
	else
		return false;

}

// Transmition from Start screen to Level Screen (if key state is Enter)
void Game::updateStartScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_RETURN))
	{
		//status = STATUS_INTRO;
		status = STATUS_INTRO;
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (ms.button_left_pressed) {
		status = STATUS_INTRO;
	}
}
void Game::updateLevelScreen()
{
	// Creates a player *only once* after 1 sec of the start of the game 

	if (!player_initializied && graphics::getGlobalTime() > 1000) {
		player = new Player(*this);
		player_initializied = true;
	}
	// update every player's move
	if (player)
		player->update();

	// Creates a meteorite, only one can exist at a time
	checkMeteorite();
	spawnMeteorite();

	if (meteorite) {

		// For every 15 points speed up
		if (meteorite->getType() == 1 && Points%15==0) {
			meteorite->speedUp();
		}
		meteorite->update();
	}

	// If they crash make an explosion effect and delete meteorite
	if (checkCollision())
	{
		// If it is a meteorite
		if (meteorite->getType() == 1) {
			graphics::playSound(std::string(ASSET_PATH) + "crash.mp3", 0.7f, false);
			CrashVocals();
			delete meteorite;
			meteorite = nullptr;
		}
		else if (meteorite->getType() == 2) // It is a healing crystal
		{
			graphics::playSound(std::string(ASSET_PATH) + "heal.wav", 0.5f, false);
			CaptureVocals();
			delete meteorite;
			meteorite = nullptr;
		}
		else if (meteorite->getType() == 3) // It is a shot
		{
			graphics::playSound(std::string(ASSET_PATH) + "shot.mp3", 1.0f, false);
			CrashVocals();
			delete meteorite;
			meteorite = nullptr;
		}
		else if (meteorite->getType() == 4)
		{
			graphics::playSound(std::string(ASSET_PATH) + "energy.wav", 0.5f, false);
			CaptureVocals();
			delete meteorite;
			meteorite = nullptr;
			if (fuel + 0.3f <= 1.0f)
			{
				fuel += 0.3f;
			}
			else {
				fuel = 1.0f;
			}

		}
	}
	 
	
}

void Game::updateGameOverScreen() {
	if (graphics::getKeyState(graphics::SCANCODE_RETURN))
	{
		// Play again
		delay1 = 0;
		Points = 0;
		fuel = 1.0f;
		player->setRemainingLife(1.0f);
		status = STATUS_PLAYING;
	}
}

void Game::update()
{
	// Start screen
	if (status == STATUS_START) {
		updateStartScreen();
	}
	else if (status == STATUS_INTRO) {
		updateIntroScreen();
	}
	else if (status == STATUS_PLAYING) {
		updateLevelScreen();
	}
	else if (status == STATUS_GAMEOVER) {
		updateGameOverScreen(); 
	}

}

void Game::updateIntroScreen() { //1
	if (graphics::getKeyState(graphics::SCANCODE_RETURN))
	{
		page += 1;
	}

}

void Game::drawIntroScreen() {

	if (page == 1) {

		// Creates a box that stores the bg image
		graphics::Brush bg;

		bg.texture = std::string(ASSET_PATH) + "CometsIntro.png";
		bg.outline_opacity = 0.0f;

		// draw background
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

		//draw static meteorite
		graphics::Brush bc;
		rotation += 0.1f * graphics::getDeltaTime();
		rotation = fmodf(rotation, 360);
		graphics::setOrientation(rotation);
		bc.outline_opacity = 0.0f;
		bc.texture = std::string(ASSET_PATH) + "Asteroid.png";
		graphics::drawRect(650, 250, 200, 200, bc);
		// Reset rotation
		graphics::resetPose();

		graphics::Brush tx;
		tx.outline_opacity = 0.0f;
		tx.fill_color[0] = 1.0f;
		tx.fill_color[1] = 0.f;
		tx.fill_color[2] = 0.f;
		char info[80];
		sprintf_s(info, "PRESS ENTER FOR NEXT PAGE >");
		graphics::drawText(CANVAS_WIDTH - 350, CANVAS_HEIGHT / 2, 20, info, tx);
	}
	else if (page == 2) {
		// Creates a box that stores the bg image
		graphics::Brush bg;

		bg.texture = std::string(ASSET_PATH) + "ShotsIntro.png";
		bg.outline_opacity = 0.0f;

		// draw background
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

		// draw shot
		graphics::Brush brush;
		graphics::resetPose();
		brush.texture = std::string(ASSET_PATH) + "shot.png";
		brush.outline_opacity = 0.0f;
		graphics::drawRect(650, 250, 130, 30, brush);

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
		graphics::drawDisk(650 - 60, 250, 60, br);

		graphics::Brush tx;
		tx.outline_opacity = 0.0f;
		tx.fill_color[0] = 1.0f;
		tx.fill_color[1] = 0.f;
		tx.fill_color[2] = 0.f;
		char info[80];
		sprintf_s(info, "PRESS ENTER FOR NEXT PAGE >");
		graphics::drawText(CANVAS_WIDTH - 350, CANVAS_HEIGHT / 2, 20, info, tx);


	}
	else if (page == 3) {
		// Creates a box that stores the bg image
		graphics::Brush bg;

		bg.texture = std::string(ASSET_PATH) + "ECrystalsIntro.png";
		bg.outline_opacity = 0.0f;

		// draw background
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

		graphics::Brush brush;
		brush.outline_opacity = 0.f;
		rotation += 0.1f * graphics::getDeltaTime();
		rotation = fmodf(rotation, 360);
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "CrystalEnergy.png";
		graphics::drawRect(650, 250, 150, 170, brush);

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
		graphics::drawDisk(650, 250, 60, br);


		// Reset rotation
		graphics::resetPose();

		graphics::Brush tx;
		tx.outline_opacity = 0.0f;
		tx.fill_color[0] = 1.0f;
		tx.fill_color[1] = 0.f;
		tx.fill_color[2] = 0.f;
		char info[80];
		sprintf_s(info, "PRESS ENTER FOR NEXT PAGE >");
		graphics::drawText(CANVAS_WIDTH - 350, CANVAS_HEIGHT / 2, 20, info, tx);


	}
	else if (page == 4) {
		// Creates a box that stores the bg image
		graphics::Brush bg;

		bg.texture = std::string(ASSET_PATH) + "HCrystalsIntro.png";
		bg.outline_opacity = 0.0f;

		// draw background
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

		graphics::Brush brush;
		rotation += 0.1f * graphics::getDeltaTime();
		rotation = fmodf(rotation, 360);
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "CrystalLife.png";
		brush.outline_opacity = 0;
		graphics::drawRect(650, 250, 100, 180, brush);

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
		graphics::drawDisk(650, 250, 60, br);

		// Reset rotation
		graphics::resetPose();

		graphics::Brush tx;
		tx.outline_opacity = 0.0f;
		tx.fill_color[0] = 1.0f;
		tx.fill_color[1] = 0.f;
		tx.fill_color[2] = 0.f;
		char info[80];
		sprintf_s(info, "PRESS ENTER FOR NEXT PAGE >");
		graphics::drawText(CANVAS_WIDTH - 350, CANVAS_HEIGHT / 2, 20, info, tx);
	}
	else if (page == 5){
	// Creates a box that stores the bg image
	graphics::Brush bg;

	bg.texture = std::string(ASSET_PATH) + "StartIntro.png";
	bg.outline_opacity = 0.0f;

	// draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

    }else {
		status = STATUS_PLAYING;
	}


}

void Game::drawGameOverScreen() {

	// Creates a box that stores the bg image
	graphics::Brush bg;

	bg.texture = std::string(ASSET_PATH) + "SpaceBG.png";
	bg.outline_opacity = 0.0f;

	// draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

	//graphics::Brush bg;

	bg.texture = std::string(ASSET_PATH) + "GameOver.png";
	bg.outline_opacity = 0.0f;


	//draw moving meteorite
	graphics::Brush bc;
	rotation += 0.1f * graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);
	graphics::setOrientation(rotation);

	if (move < 0) { move = CANVAS_WIDTH + 150; } // respawn meteorite
	move -= 2;

	bc.outline_opacity = 0.0f;
	bc.texture = std::string(ASSET_PATH) + "Asteroid.png";
	graphics::drawRect(move, 40, 150, 150, bc);
	// Reset rotation
	graphics::resetPose();

	// draw foreground
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

	// Creates text
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.f;
	br.fill_color[2] = 0.f;
	char info[40];
	string Score = "Score: " + to_string(Points);
	graphics::drawText(CANVAS_WIDTH / 2 - 100 , CANVAS_HEIGHT / 2, 30, Score, br);
	string Again = "Press Enter to retry...";
	graphics::drawText(CANVAS_WIDTH / 2 - 200 , CANVAS_HEIGHT / 2 + 50, 30, Again, br);
	init();


}

void Game::drawStartScreen()
{

	// Creates a box that stores the bg image
	graphics::Brush bg;
	bg.texture = std::string(ASSET_PATH) + "StartBG.png";
	bg.outline_opacity = 0.0f;

	// draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg);

	//draw static meteorite
	graphics::Brush bc;
	rotation += 0.1f * graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);
	graphics::setOrientation(rotation);
	bc.outline_opacity = 0.0f;
	bc.texture = std::string(ASSET_PATH) + "Asteroid.png";
	graphics::drawRect(window_width / 2 - 500, window_height / 2, 150, 150, bc);
	
	graphics::resetPose();


	// Spacecraft lights
	graphics::Brush br;
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);
	br.texture = "";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f + glow * 0.1f;
	br.fill_color[2] = 1.0f;
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 0.5f;
	br.fill_secondary_color[2] = 1.0f;
	br.fill_opacity = 1.0f;
	br.fill_secondary_opacity = 0.0f;
	br.gradient = true;
	br.outline_opacity = 0.0f;
	graphics::drawDisk(window_width / 2 + 583, window_height / 2 + 13, 18, br);
	graphics::drawDisk(window_width / 2 + 553 , window_height / 2 + 67 , 25, br);



	graphics::MouseState ms;
	graphics::getMouseState(ms);

	// TO DO : put it in a seperate method to get if the mouse position equals a text
	if ((window2canvasX(ms.cur_pos_x) > (WINDOW_WIDTH/2-100) && window2canvasX(ms.cur_pos_x) < (WINDOW_WIDTH/2+100) 
	  && window2canvasY(ms.cur_pos_y) > (WINDOW_HEIGHT / 2 - 50) && window2canvasY(ms.cur_pos_y) < (WINDOW_HEIGHT / 2 )))
	{
		graphics::Brush br;
		char info[40];
		sprintf_s(info, "START GAME");
		br.fill_color[0] = 0.0f;
		br.fill_color[1] = 0.4f;
		br.fill_color[2] = 1.0f;
		graphics::drawText(window_width / 2 - 105, window_height / 2 - 20 , 30, info, br);

	}
	else {
		graphics::Brush br;
		char info[40];
		sprintf_s(info, "START GAME");
		graphics::drawText(window_width / 2 - 105, window_height / 2 - 20, 30, info, br);
	}
}

void Game::drawLevelScreen()
{


	if (!playMusic) {
		graphics::playMusic(std::string(ASSET_PATH) + "RnM8Bit.mp3", 0.3f, true, 8000);
		playMusic = true;
	}
	// Creates a box that stores the bg image
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "SpaceBG.png";
	br.outline_opacity = 0.0f;

	// draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	// If he has 5 Energy gems draw portal
	if (energy > 4) {
		graphics::Brush brush;
		brush.texture = std::string(ASSET_PATH) + "portal.png";
		brush.outline_opacity = 0.f;
		if (zoom < 150) {
			zoom += 0.1;
		}
		graphics::drawRect(CANVAS_WIDTH - 150, CANVAS_HEIGHT / 2, zoom, zoom * 2, brush);

		PortalCollision();
	}
 
	// draw player
	if (player)
		player->draw();

	// draw meteorite
	if (meteorite)
		meteorite->draw();
	if (meteorite) {

		// Warning message : Comets are speeding up 
		if (Points%15 > 10) {

			graphics::Brush bw;

			bw.outline_opacity = 0.0f;
			bw.fill_color[0] = 1.0f;
			bw.fill_color[1] = 0.f;
			bw.fill_color[2] = 0.f;

			// draw warning msg
			graphics::drawText(CANVAS_WIDTH / 2 - 48, 40, 30,"WARNING", bw);


			bw.outline_opacity = 0.0f;
			bw.fill_color[0] = 1.f;
			bw.fill_color[1] = 1.f;
			bw.fill_color[2] = 1.f;

			graphics::drawText(CANVAS_WIDTH / 2 - 100, 60, 20, "COMETS  ARE  SPEEDING  UP!", bw);



/*
			if (Points%15 = 10) {
				// play warning sound
				graphics::playSound(std::string(ASSET_PATH) + "Warning.mp3", 0.05f, false);
			} */
		}
	}

	// Score display
#
	if (player) {
		char info[40];
		sprintf_s(info, "Score: %4d",Points);
		graphics::drawText(CANVAS_WIDTH - 185, 115, 22, info, br); // Print Score: %amount
	}

	// Display remaining fuel
	float player_fuel = player ? fuel : 0.0f;
	graphics::Brush fu;
	fu.outline_opacity = .0f;
	fu.fill_color[0] = 0.6f;
	fu.fill_color[1] = 0.0f;
	fu.fill_color[2] = 1.0f;
	fu.texture = "";
	fu.fill_secondary_color[0] = 0.2f;
	fu.fill_secondary_color[1] = 0.0f;
	fu.fill_secondary_color[2] = 1.0f;
	fu.gradient = true;
	fu.gradient_dir_u = 1.0f;
	fu.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - ((1.0f - player_fuel) * 120 / 2), 70, player_fuel * 120, 20, fu);
	fu.outline_opacity = 1.0f;
	fu.gradient = false;
	fu.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 30, 120, 20, fu);


	// Display remaining life

	float player_life = player ? player->getRemainingLife() : 0.0f;
	graphics::Brush li;
	li.outline_opacity = .0f;
	li.fill_color[0] = 0.8f;
	li.fill_color[1] = 0.5f;
	li.fill_color[2] = 0.0f;
	li.texture = "";
	li.fill_secondary_color[0] = 0.5f;
	li.fill_secondary_color[1] = 1.0f;
	li.fill_secondary_color[2] = 0.0f;
	li.gradient = true;
	li.gradient_dir_u = 1.0f;
	li.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - ((1.0f - player_life) * 120 / 2), 30, player_life * 120, 20, li);
	li.outline_opacity = 1.0f;
	li.gradient = false;
	li.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 30, 120, 20, li);

	
	// Creates a HUD
	graphics::Brush hud;
	hud.texture = std::string(ASSET_PATH) + "hud.png";
	hud.outline_opacity = 0.0f;

	// draw hud
	graphics::drawRect(CANVAS_WIDTH - 104, 60, 300, 180, hud);

	// update player
	if (player)
		player->update();

}



void Game::draw()
{
	if (status == STATUS_START)
	{
		drawStartScreen();
	}
	else if (status == STATUS_INTRO) {
		
		drawIntroScreen();
	}
	else if (status == STATUS_PLAYING) 
	{
		drawLevelScreen();
	}
	else if (status == STATUS_GAMEOVER)
	{
		drawGameOverScreen();
	}
}

void Game::init()
{
	// Font and bg music
	graphics::setFont(std::string(ASSET_PATH) + "Basic.otf");
	graphics::playMusic(std::string(ASSET_PATH) + "RnM8Bit.mp3",0.3f,true,8000);

}

float Game::window2canvasX(float x)
{
	return x * CANVAS_WIDTH / (float)window_width;
}

float Game::window2canvasY(float y)
{
	return y * CANVAS_HEIGHT / (float)window_height;
}


Game::Game()
{
}

Game::~Game()
{
	if (player)
	{
		delete player;
	}
}
