#include "GOS.h"

#pragma region Basic Functions

void GOS::Main()
{
	InitWindow(screenX, screenY, "");
	InitAudioDevice();
	SetTargetFPS(60);

	Start();

	//Audio load
	ding = LoadSound("Audio\\ding.wav");
	squeak[0] = LoadSound("Audio\\squeak1.wav");
	squeak[1] = LoadSound("Audio\\squeak2.wav");
	squeak[2] = LoadSound("Audio\\squeak3.wav");
	squeak[3] = LoadSound("Audio\\squeak4.wav");
	gameBGM = LoadSound("Audio\\mus_temvillage.ogg");
	gameOverBGM = LoadSound("Audio\\mus_gameover.ogg");

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(backgroundCol);
		Update();

		EndDrawing();
	}

	UnloadSound(ding);
	UnloadSound(squeak[0]);
	UnloadSound(squeak[1]);
	UnloadSound(squeak[2]);
	UnloadSound(squeak[3]);
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
}

void GOS::Update()
{
	EvalFrame();
	DrawFrame();
}

void GOS::EvalFrame()
{
	if (!gameOver && !IsSoundPlaying(gameBGM))
	{
		StopSound(gameOverBGM);
		PlaySound(gameBGM);
	}
	else if (gameOver && !IsSoundPlaying(gameOverBGM))
	{
		StopSound(gameBGM);
		PlaySound(gameOverBGM);
	}

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
		if (food.active)
		{
			food.Draw();
		}
		pet.Draw();
		DrawStat();
	}
	else
	{
		GameOver();
	}
}

#pragma endregion

#pragma region Additional Fucntions

void GOS::DrawStat()
{
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
		{
			if (pet.fullness >= maxStat)
				DrawRectangle(barX + barSize * i, 10, barSize, barSize, overfillStat);
			else
				DrawRectangle(barX + barSize * i, 10, barSize, barSize, RED);
		}
		else
			DrawRectangle(barX + barSize * i, 10, barSize, barSize, Color{ 230, 41, 55, 100 });

		if (i <= pet.energy)
		{
			if (pet.energy >= maxStat)
				DrawRectangle(barX + barSize * i, 30, barSize, barSize, overfillStat);
			else
				DrawRectangle(barX + barSize * i, 30, barSize, barSize, BLUE);
		}
		else
			DrawRectangle(barX + barSize * i, 30, barSize, barSize, Color{ 0, 121, 241, 100 });

		if (i <= pet.play)
		{
			if (pet.play >= maxStat)
				DrawRectangle(barX + barSize * i, 50, barSize, barSize, overfillStat);
			else
				DrawRectangle(barX + barSize * i, 50, barSize, barSize, GREEN);
		}
		else
			DrawRectangle(barX + barSize * i, 50, barSize, barSize, Color{ 0, 228, 48, 100 });

		if (i <= pet.love)
		{
			if (pet.love >= maxStat)
				DrawRectangle(barX + barSize * i, 70, barSize, barSize, overfillStat);
			else
				DrawRectangle(barX + barSize * i, 70, barSize, barSize, PINK);
		}
		else
			DrawRectangle(barX + barSize * i, 70, barSize, barSize, Color{ 255, 109, 194, 100 });
	}
}

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

	//IMPORTANT!
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

				if (playTimer >= 3)
				{
					//Stats
					PlaySound(ding);
					pet.play++;
					pet.energy--;

					playTimer = 0;
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
				if (played)
					PlaySound(squeak[GetRandomValue(0, 2)]);
				played = false;
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
			}

			stareTimer = 0;
			playTimer = 0;
		}
	}
	if (petFree || petAction[Petting])
	{
		if (CheckCollisionPointRec(GetMousePosition(), pet.body))
		{
			if (petFree || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				PlaySound(squeak[GetRandomValue(0, 2)]);
			}

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
				PlaySound(ding);
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

			do
			{
				food.GenerateFood(); //Move within food.active if statement and you get party mode
			} while (CheckCollisionRecs(food.rec, pet.body));
		}

		if (food.active)
		{
			if (CheckCollisionRecs(food.rec, pet.body))
			{
				//After eating food
				PlaySound(ding);
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

	//Details
	if (petFree)
	{
		if (CheckCollisionPointRec(GetMousePosition(), pet.leftEar) || CheckCollisionPointRec(GetMousePosition(), pet.rightEar))
		{
			pet.emote = Squint;
			pet.LookAt(GetMousePosition());

			if (timer >= 5)
			{
				timer = 0;
				PlaySound(squeak[3]);
				pet.ResetEyes();
				pet.love--;
				pet.mood = Angry;
			}
			else
			{
				timer += GetFrameTime();
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
	else if (pet.energy < 0)
	{
		gameOver = true;
		return 2; //died of exhaustion
	}
	else if (pet.play < 0)
	{
		gameOver = true;
		return 3; //died of boredom
	}
	else if (pet.love < 0)
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

void GOS::GameOver()
{
	backgroundCol = BLACK;
	int fontSize = 50;
	int fontSizeSub = 20;

	SetWindowTitle("YOU'RE A BAD OWNER.");

	//TEXT
	const char* gameover = "GAME OVER";
	DrawText(gameover,
		screenX / 2 - MeasureText(gameover, fontSize) / 2,
		screenY / 2 - fontSize,
		fontSize,
		WHITE);

	const char* deathMsg;

	switch (PetCheck())
	{
	case 1:
		deathMsg = "Your pet starved to death.";
		break;
	case 2:
		deathMsg = "Your pet died of exhaustion.";
		break;
	case 3:
		deathMsg = "Your pet was so bored it blew up.";
		break;
	case 4:
		deathMsg = "You didn't love them enough.";
		break;
	default:
		deathMsg = "Pet died.";
		break;
	}

	DrawText(deathMsg,
		screenX / 2 - MeasureText(deathMsg, fontSizeSub) / 2,
		screenY / 2 + fontSize / 2,
		fontSizeSub,
		GRAY);

	const char* subtitle = "Press ENTER to restart.";
	DrawText(subtitle,
		screenX / 2 - MeasureText(subtitle, fontSizeSub) / 2,
		screenY / 2 + fontSize,
		fontSizeSub,
		RED);

	if (IsKeyPressed(KEY_ENTER))
	{
		gameOver = false;
		Start();
	}
}

#pragma endregion

