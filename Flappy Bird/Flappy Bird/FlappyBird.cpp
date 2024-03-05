#include "FlappyBird.h"


void FlappyBird::Main()
{
	InitWindow(screen.x, screen.y, "Flappy Bird Bootleg");
	SetTargetFPS(60);

	Start();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(SKYBLUE);
		Update();
		EndDrawing();
	}

	CloseWindow();
}

void FlappyBird::Start()
{
	//Init Pipes

	//Object Pooling : active or inactive
}

void FlappyBird::Update()
{
	EvalFrame();
	DrawFrame();
}

void FlappyBird::EvalFrame()
{

}

void FlappyBird::DrawFrame()
{
	
}

void FlappyBird::InitPipes()
{

}
