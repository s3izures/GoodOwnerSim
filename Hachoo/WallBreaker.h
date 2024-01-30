#pragma once
#include "raylib.h"
#include <vector>
using namespace std;
#ifndef WALLBREAKER_H
#define WALLBREAKER_H

class WallBreaker
{
public:
	//------------VARIABLES
	Vector2 screenSize = Vector2{ 800,450 };
	bool gamePaused = false;
	bool gameOver = false;


	//Bricks
	static const int brickRows = 15;
	static const int brickCols = 15;
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
		int curLives;
		int maxLives = 3;

		void Draw()
		{
			//Position assumes middle point, Recatangle draws from upper left corner
			Rectangle rect = Rectangle{ position.x - size.x / 2, position.y - size.y / 2 ,size.x,size.y };
			DrawRectangleRounded(rect, 0.3, 8, BLACK);
		}
	} Player;

	typedef struct Ball {
		Vector2 position;
		Vector2 speed;
		int radius;
		bool active = false;

		void Draw()
		{
			Rectangle rect = Rectangle{ position.x - radius / 2, position.y - radius,(float)radius,(float)radius };
			DrawRectangleRounded(rect, 1, 8, DARKBLUE);
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

