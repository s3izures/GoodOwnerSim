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
		if (frameCounter % 10 == 0)
		{
			for (int i = player.size() - 1; i > 0; i--)
			{
				player[i].pos = player[i - 1].pos;
			}
			player[0].pos += speed;
		}
	}
	else
	{
		GameOver();
	}

	CheckBounds();
	PickupFunc();
}

void SnakeGame::DrawFrame()
{
	BeginDrawing();
	ClearBackground(screenColor);
	
	DrawGrid();

	for (Snek h : player)
	{
		h.Draw();
	}

	if (pickup.active)
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

	CheatMode();
}

void SnakeGame::CheckBounds()
{
	//Bounds
	Vector4 bounds = Vector4{ borderGap.x / 2,
							borderGap.y / 2,
							screen.x - borderGap.x / 2,
							screen.y - borderGap.y / 2 };

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
	Snek head;
	head.col = GREEN;
	head.pos = Vector2{ borderGap.x / 2,borderGap.y / 2 };
	player.push_back(head);
}

void SnakeGame::PickupFunc()
{
	if (!pickup.active)
	{
		pickup.RandPos(borderGap, screen);
		pickup.active = true;

		for (int i = 0;i<player.size();i++)
		{
			while (player[i].pos.x == pickup.pos.x && player[i].pos.y == pickup.pos.y)
				pickup.RandPos(borderGap, screen);
		}
	}

	if (player[0].pos.x == pickup.pos.x && player[0].pos.y == pickup.pos.y)
	{
		pickup.active = false;
		Snek newPiece;
		if (player.size() % 2 == 0)
			newPiece.col = LIME;
		else
			newPiece.col = DARKGREEN;
		player.push_back(newPiece);
	}

	for (int i = 1; i < player.size(); i++)
	{
		if (player[0].pos.x == player[i].pos.x && player[0].pos.y == player[i].pos.y)
		{
			gameOver = true;
		}
	}
}

void SnakeGame::GameOver()
{
	char c[] = "Game Over";
	int textSize = 24;
	DrawText(c, screen.x / 2 - (MeasureText(c, textSize) / 2), screen.y / 2, textSize, BLACK);
	if (IsKeyPressed(KEY_ENTER))
	{
		Start();
		player.clear();
		gameOver = false;
		pickup.active = false;
	}
}

void SnakeGame::CheatMode()
{
	if (IsKeyPressed(KEY_A)) //AI
	{
		std::vector<float> tilePath;

		float xTiles = (pickup.pos.x - player[0].pos.x) / tileSize;
		
		for (int i = 0; i < xTiles; i++)
		{
			if (xTiles > 0)
				tilePath.push_back(1); //right
			else
				tilePath.push_back(2); //left
		}

		float yTiles = (pickup.pos.y - player[0].pos.y) / tileSize;

		for (int i = 0; i < yTiles; i++)
		{
			if (yTiles > 0)
				tilePath.push_back(3); //down
			else
				tilePath.push_back(4); //up
		}

		int counter = 0;

		while (counter != tilePath.size())
		{
			if (tilePath[counter] == 1) //go right
			{
				
			}
			else if (tilePath[counter] == 2) //go left
			{
				
			}
			if (tilePath[counter] == 3) //go down
			{
				
			}
			else if (tilePath[counter] == 4) //go up
			{
				
			}
		}
	}
}
