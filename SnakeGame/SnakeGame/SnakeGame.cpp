#include "SnakeGame.h"

void SnakeGame::Main()
{
	InitWindow(screen.x, screen.y, "Snake Game");
	SetTargetFPS(60);

	Start();

	while (!WindowShouldClose())
	{
		Update();
	}

	CloseWindow();
}

void SnakeGame::Start()
{

}

void SnakeGame::Update()
{
	EvalFrame();
	DrawFrame();
}

void SnakeGame::EvalFrame()
{

}

void SnakeGame::DrawFrame()
{
	BeginDrawing();

	ClearBackground(screenColor);

	EndDrawing();
}
