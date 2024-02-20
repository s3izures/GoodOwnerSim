#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class SnakeGame
{
public:
	void Main();

protected:
#define tileSize 20

	bool gameOver = false;
	Vector2 screen = Vector2{ 800,600 };
	Color screenColor = RAYWHITE;
	Vector2 borderGap = Vector2{ (float( (int)(screen.x) % tileSize)), (float( (int)(screen.y) % tileSize)) };
	int frameCounter;

	void Start();
	void Update();
	void EvalFrame();
	void DrawFrame();

	typedef struct Head {
		Vector2 pos;
		Vector2 size = Vector2{ tileSize,tileSize };
		Color col;
		void Draw()
		{
			DrawRectangleV(pos, size, col);
		}
	}Head;

	std::vector<Head>player;
	Vector2 speed = Vector2{ tileSize,0 };

	typedef struct PickUp {
		bool active = false;
		Vector2 pos;
		Vector2 size = Vector2{ tileSize,tileSize };
		Color col = ORANGE;
		void Draw()
		{
			DrawRectangleV(pos, size, col);
		}
		void RandPos(Vector2 gap,Vector2 screen)
		{
			float x = GetRandomValue(0, screen.x / tileSize - 1) * tileSize + gap.x / 2;
			float y = GetRandomValue(0, screen.y / tileSize - 1) * tileSize + gap.y / 2;
			pos.x = x;
			pos.y = y;
		}
	}Pickup;

	Pickup pickup;

	void Interaction();
	void Boundaries();
	void DrawGrid();
	void InitHeadTail();
	void InitPickup();
};