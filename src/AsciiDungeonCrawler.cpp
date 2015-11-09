// AsciiDungeonCrawler.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <iostream>
#include <random>
#include <ctime>

#include "ADC_World.h"
#include "ADC_Renderer.h"
#include "ADC_Player.h"

using namespace std;

#define GAME_FPS ((float)( 10 ))
float GAME_FREQUENCY = 1.0f / GAME_FPS; // Used to limit FPS
LARGE_INTEGER FREQUENCY;

mt19937 g_TheGameRNG;

float GetCurrentGameTime(void)
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return (float)(currentTime.QuadPart) / (float)(FREQUENCY.QuadPart);
}

BOOL FPSLimit(float frameTime)
{
	enum
	{
		DONTDRAW,
		DRAW
	};

	if (GetCurrentGameTime() - frameTime < GAME_FREQUENCY) // If we are not over our threshold
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
	g_TheGameRNG.seed(static_cast<unsigned long>(time(0)));

	GAME_FREQUENCY = (float)((int)(GAME_FREQUENCY * 1000.0f)) / 1000.0f;
	QueryPerformanceFrequency(&FREQUENCY);

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

	// and then the renderer (responsible to display the game
	// and manage inputs).
	auto renderer = ADC::Renderer::CreateRendererConsole(world);
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
		if (world->getPlayer()->getLives() <= 0)
		{
			world->reset(map, 3);
		}
	});

	renderer->addOnMoveDownListener([&world]() {
		world->getPlayer()->moveDown();
	});

	renderer->addOnMoveUpListener([&world]() {
		world->getPlayer()->moveUp();
	});

	renderer->addOnMoveLeftListener([&world]() {
		world->getPlayer()->moveLeft();
	});

	renderer->addOnMoveRightListener([&world]() {
		world->getPlayer()->moveRight();
	});

	/////////////////////////////////////////////////////////////////////////////

	bool input = false;
	while (loop)
	{
		float frameTime = GetCurrentGameTime();

		input = renderer->updateInput();

		while (!FPSLimit(frameTime));

		world->update();

		if (world->getPlayer()->hasBeenUpdated(true))
		{
			renderer->updateGUI(*world->getPlayer());
			if (world->getPlayer()->getLives() == 0)
			{
				renderer->playerDead();
			}
		}
		if (input || world->hasBeenUpdated(false))
		{
			renderer->render();
		} 
	}

	return 0;
}

