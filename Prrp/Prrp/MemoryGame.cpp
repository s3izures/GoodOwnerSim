#include "MemoryGame.h"
#include <iostream>
void MemoryGame::Main()
{
	Start();

	InitWindow(screenWidth, screenHeight, "Memory Game");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);
		Update();
		EndDrawing();
	}

	CloseWindow();
}
void MemoryGame::Start()
{
	int maxSize = (gridSize * gridSize / 2);
	vector<int> numbers;
	for (int i = 0; i < maxSize; i++)
	{
		numbers.push_back(i + 1);
		numbers.push_back(i + 1);
	}

	for (int row = 0; row < gridSize; row++)
	{
		for (int col = 0; col < gridSize; col++)
		{
			float x = gap + (gap + tileSize) * col;
			float y = gap + (gap + tileSize) * row;
			Rectangle rect = Rectangle{ x,y,tileSize, tileSize };

			int randId = rand() % numbers.size();
			int num = numbers[randId];

			Tile tile = { num,rect,GRAY,36 };
			tiles.push_back(tile);

			numbers.erase(numbers.begin() + randId);
		}
	}
}
void MemoryGame::Update()
{
	EvalFrame();
	DrawFrame();
}
void MemoryGame::EvalFrame()
{
	canClick = true;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && canClick)
	{
		Vector2 mousePoint = GetMousePosition();

		for (int i = 0; i < tiles.size(); i++)
		{
			if (CheckCollisionPointRec(mousePoint, tiles[i].rect))
			{
				if (tiles[i].state == hidden)
				{
					tiles[i].state = revealed;
					Tile* til = &tiles[i];
					matching.push_back(til);

					//remember time
					timeOfClick = GetTime();
				}
			}
		}
	}

	if (GetTime() > timeOfClick + 1)
	{
		if (matching.size() == 2)
		{
			if (matching[0]->num == matching[1]->num)
			{
				matching[0]->state = found;
				matching[1]->state = found;
			}
			else
			{
				matching[0]->state = hidden;
				matching[1]->state = hidden;
			}
			matching.clear();
		}
	}
}
void MemoryGame::DrawFrame()
{
	for (Tile i : tiles)
	{
		i.DetState();
		i.Draw();
		i.DrawNum();
	}
}
