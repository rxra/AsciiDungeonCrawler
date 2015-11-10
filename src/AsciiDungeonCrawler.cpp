// AsciiDungeonCrawler.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <iostream>
#include <ctime>

#include "ADC_World.h"
#include "ADC_Renderer.h"
#include "ADC_Player.h"
#include "ADC_RandomGenerator.h"

using namespace std;

const float kGameFPS = 10.f;
float kGameFrequency = 1.0f / kGameFPS;
LARGE_INTEGER frequency;
const float kMonstersSpawnFrequency = 10;


float GetCurrentGameTime(void)
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return (float)(currentTime.QuadPart) / (float)(frequency.QuadPart);
}

BOOL FPSLimit(float frameTime)
{
	enum
	{
		DONTDRAW,
		DRAW
	};

	if (GetCurrentGameTime() - frameTime < kGameFrequency) // If we are not over our threshold
	{
		return DONTDRAW;
	}
	else
	{
		return DRAW;
	}
}

int main(int argc, const char** argv)
{
	kGameFrequency = (float)((int)(kGameFrequency * 1000.0f)) / 1000.0f;
	QueryPerformanceFrequency(&frequency);

	string map("test1.map");

	// create the world
	auto world = ADC::World::Create(map, 3);
	if (world==nullptr)
	{
		cerr << "World is not valid!" << endl;
		cerr << "Abord!" << endl;
		cerr << "Press any key!" << endl;
		getchar();
		exit(1);
	}

	ADC::RandomGenerator::Initialize();

	// and then the renderer (responsible to display the game
	// and manage inputs).
	auto renderer = ADC::Renderer::CreateRendererConsole(world,64,64);
	if (renderer==nullptr)
	{
		cerr << "Failed to initialize renderer!" << endl;
		cerr << "Abord!" << endl;
		cerr << "Press any key!" << endl;
		getchar();
		exit(1);
	}

	/////////////////////////////////////////////////////////////////////////////
	// setup some listeners

	bool loop = true;
	renderer->addOnQuitListener([&loop]() {
		loop = false;
	});

	renderer->addOnRestartListener([&world,&map]() {
		if (world->getPlayer()->getLives() <= 0 || world->getPlayer()->win())
		{
			world->reset(map, 3);
		}
	});

	renderer->addOnMoveDownListener([&world]() {
		if (world->getPlayer()->getLives() > 0 && !world->getPlayer()->win())
		{
			world->getPlayer()->moveDown();
		}
	});

	renderer->addOnMoveUpListener([&world]() {
		if (world->getPlayer()->getLives() > 0 && !world->getPlayer()->win())
		{
			world->getPlayer()->moveUp();
		}
	});

	renderer->addOnMoveLeftListener([&world]() {
		if (world->getPlayer()->getLives() > 0 && !world->getPlayer()->win())
		{
			world->getPlayer()->moveLeft();
		}
	});

	renderer->addOnMoveRightListener([&world]() {
		if (world->getPlayer()->getLives() > 0 && !world->getPlayer()->win())
		{
			world->getPlayer()->moveRight();
		}
	});

	/////////////////////////////////////////////////////////////////////////////

	bool input = false;
	float lastMonstersSpawntime = GetCurrentGameTime();

	while (loop)
	{
		float frameTime = GetCurrentGameTime();

		input = renderer->updateInput();

		if (world->getPlayer()->getLives() > 0 && !world->getPlayer()->win())
		{
			world->update();
		}

		while (!FPSLimit(frameTime));

		if ((GetCurrentGameTime() - lastMonstersSpawntime) >= kMonstersSpawnFrequency)
		{
			lastMonstersSpawntime = GetCurrentGameTime();
			world->spawnMonster();
		}

		if (world->getPlayer()->hasBeenUpdated(true))
		{
			renderer->updateReference();
			renderer->updateGUI(*world->getPlayer());
		}
		
		if (input || world->hasBeenUpdated(false))
		{
			if (world->getPlayer()->getLives() == 0)
			{
				renderer->playerDead();
			}
			else if (world->getPlayer()->win())
			{
				renderer->playerWin();

			}
			renderer->updateReference();
			renderer->render(world->getPlayer()->getLives() > 0 && !world->getPlayer()->win());
		} 
	}

	return 0;
}

