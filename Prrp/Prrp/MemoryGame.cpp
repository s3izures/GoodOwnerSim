#include "MemoryGame.h"

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
			Rectangle rect = Rectangle{ x,y,tileSize };

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

}
void MemoryGame::DrawFrame()
{
	for (Tile i : tiles)
	{
		i.Draw();
	}
}
