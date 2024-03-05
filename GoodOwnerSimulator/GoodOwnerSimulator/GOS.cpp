#include "GOS.h"

void GOS::Main()
{
	InitWindow(screenX, screenY, "Good Owner Simulator");
	SetTargetFPS(60);
	
	Start();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		Update();
		EndDrawing();
	}

	CloseWindow();
}

void GOS::Start()
{
}

void GOS::Update()
{
	EvalFrame();
	DrawFrame();

	//use GetTime() for timers
}

void GOS::EvalFrame()
{

}

void GOS::DrawFrame()
{
	//Start Menu
	//Pet Customization
	//Gameplay
	//Gameover
}
