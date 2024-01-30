#pragma once
#include "raylib.h"
#include <vector>
using namespace std;
#ifndef WALLBREAKER_H
#define WALLBREAKER_H

class WallBreaker
{
#define FIX_SPEED GetFrameTime() * 50

public:
	//------------VARIABLES
	Vector2 screenSize = Vector2{ 800,450 };
	bool gamePaused = false;
	bool gameOver = false;
	bool levelWin = false;


	//Bricks
	static const int brickRows = 6;
	static const int brickCols = 10;
	float brickHeight = 30;
	float gap = 10;

	typedef struct Brick {
		Color color;
		Rectangle rect;
		void Draw()
		{
			DrawRectangleRounded(rect, 0.5, 12, color);
		}
	}Brick;

	vector<Brick> bricks;
	Color rowColor[brickRows] = { RED,ORANGE,YELLOW,GREEN,BLUE,PURPLE };


	//Player + Ball
	typedef struct Player {
		Vector2 position;
		Vector2 size;
		int speed = 8;
		int curLives;
		int maxLives = 3;
		
		void Draw()
		{
			//Position assumes middle point, Recatangle draws from upper left corner
			DrawRectangleRounded(getRect(), 0.3, 8, BLACK);
		}
		Rectangle getRect()
		{
			Rectangle rect = Rectangle{ position.x - size.x / 2, position.y - size.y / 2 ,size.x,size.y };
			return rect;
		}
	} Player;

	typedef struct Ball {
		Vector2 position;
		Vector2 speed;
		int radius;
		bool active = false;

		void Draw()
		{
			DrawCircle(position.x, position.y, radius, DARKBLUE);
		}
	} Ball;

	Player player = { 0 };
	Ball ball = { 0 };


	//------------FUNCTIONS
	void Main();
	void Start();
	void EvalCurFrame();
	void DrawCurFrame();
	void Update();

	//EXTRA FUNCTIONS
	void MakeBricks();
	void MakePlayer();
	void MakeBall();
};

#endif

