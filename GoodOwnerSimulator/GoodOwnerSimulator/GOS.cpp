#include "GOS.h"

#pragma region Basic Functions

void GOS::Main()
{
	InitWindow(screenX, screenY, "Good Owner Simulator");
	SetTargetFPS(60);

	Start();

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(DARKGRAY);
		Update();

		EndDrawing();
	}

	CloseWindow();
}

void GOS::Start()
{
	pet = Pet();
}

void GOS::Update()
{
	EvalFrame();
	DrawFrame();

	//use GetTime() for timers
	//use GetFrameTime() for animations?
}

void GOS::EvalFrame()
{

}

void GOS::DrawFrame()
{
	if (!gameOver)
	{
		pet.Draw();
		pet.LookAtMouse();
	}
}

#pragma endregion

#pragma region Additional Fucntions



#pragma endregion

