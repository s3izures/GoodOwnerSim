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
	int brickRows = 6;
	int brickCols = 10;
	float brickHeight = 24;
	float gap = 8;

	typedef struct Brick {
		Color color;

		//Collisions
		Rectangle rect;
		Rectangle hitboxes[8];

		void Draw()
		{
			DrawRectangleRounded(rect, 0.5, 12, color);
		}
		void DrawCollider()
		{
			for (int i = 0; i < 8; i++)
			{
				if (i < 4 || i > 5)
				{
					DrawRectangleRec(hitboxes[i], Color{ 255,255,255,200 });
				}
				else
				{
					DrawRectangleRec(hitboxes[i], Color{ 0,0,0,0 });
				}
			}
		}
	}Brick;

	vector<Brick> bricks;
	vector<Color> brickColor;

	enum BoxSides {
		tLCorn,
		tRCorn,
		bLCorn,
		bRCorn,
		top,
		bottom,
		left,
		right,
		end
	};

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
		Vector2 prevPosition;
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


	//Pickups
	typedef struct Pickup {
		Vector2 position;
		Vector2 speed;
		int radius;
		Color color;

		void Draw() 
		{ 
			DrawCircle(position.x, position.y, radius, color); 
		}
	} PickUp;

	vector<Pickup> pickups;



	//------------FUNCTIONS
	void Main();
	void Start();
	void EvalCurFrame();
	void DrawCurFrame();
	void Update();

	//EXTRA FUNCTIONS
	void Restart();
	void GenerateLevel();

	void MakeBricks();
	void MakePlayer();
	void MakeBall();

	void CollisionPaddle();
	void CollisionWalls();
	void CollisionBall();
	int CollisionWithHitBox(Brick brick);
};

#endif

