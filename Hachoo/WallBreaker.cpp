#include "WallBreaker.h"
#include "raylib.h"
#include <vector>
using namespace std;

void WallBreaker::Main()
{
	InitWindow(screenSize.x, screenSize.y, "Wallbreaker");
	Start();
	SetTargetFPS(60);
	while(!WindowShouldClose())
	{
		Update();
	}
	CloseWindow();
}

void WallBreaker::Start()
{
	MakeBricks();
	MakePlayer();
	MakeBall();
}

void WallBreaker::EvalCurFrame()
{
	//Game Over and Paused
	if (gameOver)
	{
		if (IsKeyPressed(KEY_ENTER))
		{
			Start();
			gameOver = false;
		}
		return;
	}
	if (IsKeyPressed(KEY_P))
	{
		gamePaused = !gamePaused;
	}
	if (gamePaused)
	{
		return;
	}

	//Balls
	if (!ball.active)
	{
		ball.position = Vector2{ player.position.x,screenSize.y * 9 / 10 - ball.radius };
		if (IsKeyPressed(KEY_SPACE))
		{
			ball.active = true;
		}
	}
	else
	{
		ball.position.x += ball.speed.x;
		ball.position.y += ball.speed.y;
	}

	//Player
	if (IsKeyDown(KEY_LEFT) && player.position.x > 0 )
	{
		player.position.x -= 3;
	}
	else if (IsKeyDown(KEY_RIGHT) && player.position.x + player.size.x < (int)GetScreenWidth() - 5)
	{
		player.position.x += 3;
	}
}

void WallBreaker::DrawCurFrame()
{
	BeginDrawing();
	ClearBackground(WHITE);

	if (!gameOver)
	{
		for (Brick b : bricks)
		{
			b.Draw();
		}
		player.Draw();
		ball.Draw();
	}
	else
	{
		DrawText("Press Enter to play again", GetScreenWidth() / 2 - (MeasureText("Press Enter to play again", 20)/2), GetScreenHeight() / 2 - 20, 20, BLACK);
	}

	EndDrawing();
}

void WallBreaker::Update()
{
	EvalCurFrame();
	DrawCurFrame();
}

void WallBreaker::MakeBricks()
{
	float spaceForBricks = screenSize.x - (brickCols * gap + gap);
	Vector2 brickSize = Vector2{ spaceForBricks / brickCols, brickHeight };

	for (int row = 0; row < brickRows; row++)
	{
		for (int col = 0; col < brickCols; col++)
		{
			float x = gap + (gap + brickSize.x) * col;
			float y = gap + (gap + brickSize.y) * row;

			Rectangle brick = Rectangle{ x,y,brickSize.x,brickSize.y };
			Brick brickk = Brick{ rowColor[row],brick };

			bricks.push_back(brickk);
		}
	}
}

void WallBreaker::MakePlayer()
{
	player.position = Vector2{ screenSize.x / 2,screenSize.y * 9 / 10 };
	player.size = { screenSize.x / 10,20 };
	player.curLives = player.maxLives;
}

void WallBreaker::MakeBall()
{
	ball.position = Vector2{ player.position.x,player.position.y - 30 };
	ball.speed = Vector2{ 0,-3 };
	ball.radius = 20;
}