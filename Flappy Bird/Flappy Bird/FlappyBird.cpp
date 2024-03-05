#include "FlappyBird.h"


void FlappyBird::Main()
{
	InitWindow(screenX, screenY, "Flappy Bird Bootleg");
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
	InitPipes();

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
	for (Pipe p : pipes)
	{
		p.Draw();
	}
}

void FlappyBird::InitPipes()
{
	for (int i = 0; i < 5; i++)
	{
		float x = 300 + gap * i;
		Pipe p = Pipe(x);
		pipes.push_back(p);
	}
}
