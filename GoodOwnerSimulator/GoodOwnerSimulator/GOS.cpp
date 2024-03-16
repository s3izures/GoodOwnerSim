#include "GOS.h"

#pragma region Basic Functions

void GOS::Main()
{
	InitWindow(screenX, screenY, "");
	SetTargetFPS(60);

	Start();

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(backgroundCol);
		Update();

		EndDrawing();
	}

	CloseWindow();
}

void GOS::Start()
{
	SetWindowTitle("Good Owner Simulator");
	
	pet = Pet();

	//Timer reset
	timer = 0;
	stareTimer = 0;
	playTimer = 0;
	pettingTimer = 0;

	//Background reset
	backgroundCol = Color{ 204, 198, 190, 255 };

	//Load Music
}

void GOS::Update()
{
	EvalFrame();
	DrawFrame();
}

void GOS::EvalFrame()
{
	PetCheck();
	PetInteraction();

	decay += GetFrameTime();
	
	if (!petAction[Playing])
		energyRegen += GetFrameTime();

	if (energyRegen >= 10 && !petAction[Eating])
	{
		pet.energy++;
		pet.fullness--;
		energyRegen = 0;
	}
	if (decay >= 30)
	{
		pet.love--;
		pet.play--;
		decay = 0;
	}
}

void GOS::DrawFrame()
{
	if (!gameOver)
	{
		pet.Draw();
		if (food.active)
		{
			food.Draw();
		}

		//BARS
		DrawText("Fullness", 10, 10, 14, BLACK);
		DrawText("Energy", 10, 30, 14, BLACK);
		DrawText("Fun", 10, 50, 14, BLACK);
		DrawText("Love", 10, 70, 14, BLACK);

		float longestText = MeasureText("Fullness", 14) + 10;
		float gap = 5;
		float barSize = 10;
		float barX = longestText + gap;
		float barY = 10; //origin

		for (int i = 0; i < maxStat; i++)
		{
			if (i <= pet.fullness)
				DrawRectangle(barX + barSize * i, 10, barSize, barSize, RED);
			else
				DrawRectangle(barX + barSize * i, 10, barSize, barSize, Color{ 230, 41, 55, 100 });

			if (i <= pet.energy)
				DrawRectangle(barX + barSize * i, 30, barSize, barSize, BLUE);
			else
				DrawRectangle(barX + barSize * i, 30, barSize, barSize, Color{ 0, 121, 241, 100 });
			
			if (i <= pet.play)
				DrawRectangle(barX + barSize * i, 50, barSize, barSize, GREEN);
			else
				DrawRectangle(barX + barSize * i, 50, barSize, barSize, Color{ 0, 228, 48, 100 });

			if (i <= pet.love)
				DrawRectangle(barX + barSize * i, 70, barSize, barSize, PINK);
			else
				DrawRectangle(barX + barSize * i, 70, barSize, barSize, Color{ 255, 109, 194, 100 });
		}
	}
	else
	{
		backgroundCol = BLACK;
		int fontSize = 50;
		int fontSizeSub = 20;

		SetWindowTitle("YOU'RE A BAD OWNER.");

		//TEXT
		const char* gameover= "GAME OVER";
		DrawText(gameover,
			screenX / 2 - MeasureText(gameover, fontSize) / 2,
			screenY / 2 - fontSize,
			fontSize,
			WHITE);

		const char* subtitle = "Press ENTER to restart.";
		DrawText(subtitle,
			screenX / 2 - MeasureText(subtitle, fontSizeSub) / 2,
			screenY / 2 + fontSize / 2,
			fontSizeSub,
			RED);

		if (IsKeyPressed(KEY_ENTER))
		{
			gameOver = false;
			Start();
		}
	}
}

#pragma endregion

#pragma region Additional Fucntions

void GOS::PetMove(Vector2 pos)
{
	//Move TOWARDS pos
	if (pet.position.x < pos.x && pet.position.x + pet.bodySize / 2 <= screenX)
		pet.position.x += pet.speed.x;
	if (pet.position.x > pos.x && pet.position.x - pet.bodySize / 2 >= 0)
		pet.position.x -= pet.speed.x;
	if (pet.position.y < pos.y && pet.position.y + pet.bodySize / 2 <= screenY)
		pet.position.y += pet.speed.y;
	if (pet.position.y > pos.y && pet.position.y - pet.earSize.x >= 0)
		pet.position.y -= pet.speed.y;

	pet.Move(pos);
}

void GOS::PetInteraction()
{
	if (petFree || petAction[Playing])
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			petAction[Playing] = true;
			petFree = false;

			Vector2 mousePoint = GetMousePosition();
			pet.LookAt(mousePoint);

			//Expression
			pet.emote = Squint;

			stareTimer += GetFrameTime();

			if (stareTimer >= 2)
				pet.mood = Curious;

			if (stareTimer >= 3)
			{
				PetMove(mousePoint);
				playTimer += GetFrameTime();

				if (playTimer >= 3 && canPlay)
				{
					//Stats
					pet.play++;
					pet.energy--;

					canPlay = false;
					played = true;
				}
				else
				{
					//Expression
					pet.emote = Neutral;
				}
			}
		}
		else if (petFree == false)
		{
			pet.ResetEyes();

			timer += GetFrameTime();

			if (timer <= 3 && played)
			{
				pet.emote = ClosedUp;
				pet.mood = Energetic;
			}
			else
			{
				timer = 0;

				pet.emote = Neutral;
				pet.mood = Default;

				petAction[Playing] = false;
				petFree = true;
				played = false;
				canPlay = true;
			}

			stareTimer = 0;
			playTimer = 0;
		}
	}
	if (petFree || petAction[Petting])
	{
		if (CheckCollisionPointRec(GetMousePosition(), pet.body))
		{
			petAction[Petting] = true;
			petFree = false;

			pet.ResetEyes();
			pettingTimer += GetFrameTime();

			if (canPet)
			{
				pet.emote = ClosedDown; //not petted but petting
			}
			else
			{
				pet.mood = Happy;
				pet.emote = ClosedUp; //petted
			}

			if (pettingTimer >= 4 && canPet)
			{
				pet.love++;
				canPet = false;
			}
		}
		else if (petFree == false)
		{
			pettingTimer = 0;
			pet.mood = Default;
			pet.emote = Neutral;

			petAction[Petting] = false;
			petFree = true;
			canPet = true;
		}
	}
	if (petFree || petAction[Eating])
	{
		//Spawn food
		if (IsKeyPressed(KEY_E))
		{
			petAction[Eating] = true;
			petFree = false;
			food.active = true;

			food.GenerateFood(); //Move within food.active if statement and you get party mode
		}

		if (food.active)
		{
			if (CheckCollisionRecs(food.rec, pet.body))
			{
				//After eating food
				pet.fullness++;
				food.active = false;

				pet.ResetEyes();
			}
			else
			{
				pet.LookAt(food.position);
				PetMove(food.position);
			}
		}

		if (!food.active && petAction[Eating])
		{
			if (timer <= 3)
			{
				timer += GetFrameTime();
				pet.emote = ClosedUp;
				pet.mood = Happy;
			}
			else
			{
				timer = 0;

				pet.emote = Neutral;
				pet.mood = Default;

				petAction[Eating] = false;
				petFree = true;
			}
		}
	}
	
}

int GOS::PetCheck()
{
	if (pet.fullness < 0)
	{
		gameOver = true;
		return 1; //died of hunger
	}
	if (pet.energy < 0)
	{
		gameOver = true;
		return 2; //died of exhaustion
	}
	if (pet.play < 0)
	{
		gameOver = true;
		return 3; //died of boredom
	}
	if (pet.love < 0)
	{
		gameOver = true;
		return 4; //died of sadness
	}

	if (pet.fullness <= 3)
	{
		pet.emote = Squint;
		pet.mood = Hungry;
	}
	else if (pet.energy <= 3)
	{
		pet.emote = Squint;
		pet.mood = Tired;
	}
	else if (pet.play <= 3)
	{
		pet.emote = Squint;
		pet.mood = Sad;
	}
	else if (pet.love <= 3)
	{
		pet.emote = ClosedDown;
		pet.mood = Sad;
	}

	return 0; //Alive
}

#pragma endregion

