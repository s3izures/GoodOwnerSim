#include "SnakeGame.h"

Vector2& operator+= (Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

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
	InitHeadTail();
}

void SnakeGame::Update()
{
	EvalFrame();
	DrawFrame();
}

void SnakeGame::EvalFrame()
{
	if (!gameOver)
	{
		frameCounter++;

		Interaction();

		//Move head
		if (frameCounter % 5 == 0)
		{
			player[0].pos += speed;
		}
	}
	else
	{
		char c[] = "Game Over";
		int textSize = 24;
		DrawText(c, screen.x / 2 - (MeasureText(c, textSize) / 2), screen.y / 2, textSize, BLACK);
	}

	Boundaries();

	if (!pickup.active)
	{
		pickup.RandPos(borderGap, screen);
		pickup.active = true;
	}
}

void SnakeGame::DrawFrame()
{
	BeginDrawing();
	ClearBackground(screenColor);
	
	DrawGrid();

	for (Head h : player)
	{
		h.Draw();
	}

	pickup.Draw();

	EndDrawing();
}

void SnakeGame::Interaction()
{
	if (IsKeyPressed(KEY_RIGHT))
	{
		if (speed.x == 0)
			speed = Vector2{ tileSize,0 };
	}
	if (IsKeyPressed(KEY_LEFT))
	{
		if (speed.x == 0)
			speed = Vector2{ -tileSize,0 };
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		if (speed.y == 0)
			speed = Vector2{ 0,tileSize };
	}
	if (IsKeyPressed(KEY_UP))
	{
		if (speed.y == 0)
			speed = Vector2{ 0,-tileSize };
	}
}

void SnakeGame::Boundaries()
{
	//Bounds
	Vector4 bounds = Vector4{ borderGap.x / 2,borderGap.y / 2,
							screen.x - borderGap.x / 2,screen.y - borderGap.x / 2 };

	//Check
	if (player[0].pos.x < bounds.x || player[0].pos.x >= bounds.z
		|| player[0].pos.y < bounds.y || player[0].pos.y >= bounds.w)
	{
		gameOver = true;
	}
}

void SnakeGame::DrawGrid()
{
	//Vertical Lines
	for (int i = 0; i < screen.x / tileSize + 1; i++)
	{
		Vector2 start = Vector2{ borderGap.x / 2 + tileSize * i,borderGap.y / 2 };
		Vector2 end = Vector2{ borderGap.x / 2 + tileSize * i, screen.y - borderGap.y / 2 };
		DrawLineV(start, end, WHITE);
	}
	//Horizontal Lines
	for (int i = 0; i < screen.y / tileSize + 1; i++)
	{
		Vector2 start = Vector2{ borderGap.x / 2 ,borderGap.y / 2 + tileSize * i };
		Vector2 end = Vector2{ screen.x - borderGap.x / 2 ,borderGap.y / 2 + tileSize * i };
		DrawLineV(start, end, WHITE);
	}
}

void SnakeGame::InitHeadTail()
{
	Head head;
	head.col = RED;
	head.pos = Vector2{ borderGap.x / 2,borderGap.y / 2 };
	player.push_back(head);
}

void SnakeGame::InitPickup()
{
}
