#pragma once
#include "raylib.h"

class SnakeGame
{
public:
	Vector2 screen = { 800,600 };
	Color screenColor = RAYWHITE;
	int tileSize;


	void Main();
	void Start();
	void Update();
	void EvalFrame();
	void DrawFrame();
};