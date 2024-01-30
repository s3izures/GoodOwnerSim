#include "WallBreaker.h"
#include "raylib.h"
#include <vector>
using namespace std;

void WallBreaker::Main()
{
	InitWindow(screenSize.x, screenSize.y, "Wallbreaker");
	Start();
	SetTargetFPS(120);
	while(!WindowShouldClose())
	{
		Update();
	}
	CloseWindow();
}

void WallBreaker::Start()
{
	//Generate Level
	

	//Make stuff
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
		ball.position = Vector2{ player.position.x,screenSize.y * 9 / 10 - ball.radius - 30 };
		if (IsKeyPressed(KEY_SPACE))
		{
			ball.active = true;
		}
	}
	else
	{
		ball.position.x += ball.speed.x * FIX_SPEED;
		ball.position.y += ball.speed.y * FIX_SPEED;
	}

	//Player controls
	if (IsKeyDown(KEY_LEFT) && player.position.x - (player.size.x/2) > 0 )
	{
		player.position.x -= player.speed * FIX_SPEED;
	}
	else if (IsKeyDown(KEY_RIGHT) && player.position.x + (player.size.x / 2) < (int)GetScreenWidth() - 5)
	{
		player.position.x += player.speed * FIX_SPEED;
	}

	//BALL collision
	for (int i = 0; i < bricks.size(); i++)
	{
		if (CheckCollisionCircleRec(ball.position, ball.radius, bricks[i].rect))
		{
			//Delete brick
			bricks.erase(bricks.begin() + i);

			//Reverse ball
			ball.speed.y *= -1;

			break;
		}
	}

	//Wall collision
	if (ball.position.y <= 0 + ball.radius)
	{
		//Reverse ball
		ball.speed.y *= -1;
	}
	if (ball.position.x <= 0 + ball.radius || ball.position.x >= GetScreenWidth() - ball.radius)
	{
		//Reverse ball
		ball.speed.x *= -1;
	}
	if (ball.position.y >= GetScreenHeight() + 20)
	{
		player.curLives--;
		ball.active = false;
		ball.speed = { 0,-5 };
	}

	//Paddle collision
	if (CheckCollisionCircleRec(ball.position, ball.radius, player.getRect()))
	{
		ball.speed.y *= -1;
		ball.speed.x = (ball.position.x - player.position.x) / (player.size.x / 10);
	}

	//LOSS/WIN
	if (player.curLives <= 0)
	{
		gameOver = true;
		bricks.clear();
	}
	else
	{
		if (bricks.size() <= 0)
		{
			levelWin = true;
		}
	}
}

void WallBreaker::DrawCurFrame()
{
	BeginDrawing();
	ClearBackground(WHITE);

	if (!gameOver && !levelWin)
	{
		//Draw lives
		DrawText("LIVES", 20, GetScreenHeight() - 20 - 20, 18, BLACK);
		for (int i = 0; i < player.maxLives; i++)
		{
			if (i < player.curLives)
			{
				DrawRectangle(20 + 20 * i, GetScreenHeight() - 20, 10, 10, DARKBLUE);
			}
			else
			{
				DrawRectangle(20 + 20 * i, GetScreenHeight() - 20, 10, 10, LIGHTGRAY);
			}
		}

		//Draw things
		for (Brick b : bricks)
		{
			b.Draw();
		}
		player.Draw();
		ball.Draw();
	}
	else if (levelWin)
	{
		DrawText("Press Enter to go to the next level", GetScreenWidth() / 2 - (MeasureText("Press Enter to go to the next level", 20) / 2), GetScreenHeight() / 2 - 20, 20, BLACK);
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
			Brick brickk = Brick{ rowColor[row%6],brick };

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
	ball.speed = Vector2{ 0,-5 };
	ball.radius = 10;
}