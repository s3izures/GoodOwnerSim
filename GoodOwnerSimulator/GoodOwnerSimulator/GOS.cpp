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
	stareTimer = 0;
	playTimer = 0;
	pettingTimer = 0;

	//Background reset
	backgroundCol = LIGHTGRAY;
}

void GOS::Update()
{
	EvalFrame();
	DrawFrame();
}

void GOS::EvalFrame()
{

	PetInteraction();
}

void GOS::DrawFrame()
{
	if (!gameOver)
	{
		pet.Draw();
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

		//Do custom gameover screens
		const char* subtitle = "THIS IS ALL YOUR FAULT THIS IS ALL YOUR FAULT THIS IS ALL YOUR FAULT";
		DrawText(subtitle,
			screenX / 2 - MeasureText(subtitle, fontSizeSub) / 2,
			screenY / 2 + fontSize / 2,
			fontSizeSub,
			RED);
	}
}

#pragma endregion

#pragma region Additional Fucntions

void GOS::PetInteraction()
{
	//Play
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePoint = GetMousePosition();
		pet.LookAt(mousePoint);

		stareTimer += GetFrameTime();

		if (stareTimer >= 4)
		{
			pet.Move(mousePoint);
			playTimer += GetFrameTime();

			if (playTimer >= 5 && canPlay)
			{
				pet.play++;
				pet.energy--;
				cout << pet.play;
				canPlay = false;
			}
		}
	}
	else
	{
		canPlay = true;
		stareTimer = 0;
		playTimer = 0;
		pet.emote = Neutral;
		pet.mood = Default;
		pet.ResetEyes();
	}

	//Petting
	if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), pet.body))
	{
		petBusy = true;

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
			cout << pet.love;
			canPet = false;
		}
	}
	else
	{
		canPet = true;
		pettingTimer = 0;
		pet.mood = Default;
		pet.emote = Neutral;
		petBusy = false;
	}
}

int GOS::PetCheck()
{
	if (pet.fullness <= 0)
	{
		gameOver = true;
		return 1; //died of hunger
	}
	if (pet.energy <= 0)
	{
		gameOver = true;
		return 2; //died of exhaustion
	}
	if (pet.play <= 0)
	{
		gameOver = true;
		return 3; //died of boredom
	}
	if (pet.love <= 0)
	{
		gameOver = true;
		return 4; //died of sadness
	}

	if (pet.fullness >= 420)
	{
		gameOver = true;
		return 5; //died of overfeeding
	}
	if (pet.play <= 21)
	{
		gameOver = true;
		return 6; //died of excitement
	}
	if (pet.love <= 69)
	{
		gameOver = true;
		return 7; //died of too much love
	}

	if (pet.fullness <= 5)
	{
		pet.emote = Squint;
		pet.mood = Hungry;
	}
	else if (pet.energy <= 5)
	{
		pet.emote = Squint;
		pet.mood = Tired;
	}
	else if (pet.play <= 5)
	{
		pet.emote = Squint;
		pet.mood = Sad;
	}
	else if (pet.love <= 5)
	{
		pet.emote = ClosedDown;
		pet.mood = Sad;
	}

	return 0; //Alive
}

#pragma endregion

