
#include "ADC_Monster.h"
#include "ADC_Player.h"
#include <Windows.h>
#include <random>

using namespace std;

extern mt19937 g_TheGameRNG;

namespace ADC
{

	bool Monster::movePossible(World::Cell cell, bool& consummed)
	{
		consummed = false;

		if (cell == World::Cell::ConcreteWall	||
			cell == World::Cell::BrickWall		||
			cell == World::Cell::IronDoor		||
			cell == World::Cell::BronzeDoor		||
			cell == World::Cell::NaughtyMonster)
		{
			return false;
		}

		return true;
	}

	bool Monster::update()
	{
		int sd = getPosition().sqrDistance(_world->getPlayer()->getPosition());
		if (sd <= _sqrDistanceToPlayer)
		{
			_getCloserToPlayer();
		}
		else
		{
			_randomMove();
		}

		if (_currentPosition == _world->getPlayer()->getPosition())
		{
			bool monsterDie = false;
			_world->getPlayer()->hittedByMonster(monsterDie);
			if (monsterDie)
			{
				_world->getPlayer()->killMonster();
				return false;
			}
			else
			{
				_previousWorldCell = World::Cell::FreeCell;
			}
		}

		return true;
	}

	void Monster::_getCloserToPlayer()
	{
		int dx = abs(_currentPosition.x - _world->getPlayer()->getPosition().x);
		int dy = abs(_currentPosition.y - _world->getPlayer()->getPosition().y);
		if ((dx < dy) || (dy == 0))
		{
			if (_currentPosition.x < _world->getPlayer()->getPosition().x)
			{
				moveRight();
			}
			else
			{
				moveLeft();
			}
		}
		else
		{
			if (_currentPosition.y < _world->getPlayer()->getPosition().y)
			{
				moveDown();
			}
			else
			{
				moveUp();
			}
		}
	}

	void Monster::_randomMove()
	{
		bool moved = false;
		uniform_int_distribution<uint32_t> dist4(0, 3);
		switch (dist4(g_TheGameRNG)) {
		case 0:
			moved = moveUp();
			break;
		case 1:
			moved = moveDown();
			break;
		case 2:
			moved = moveLeft();
			break;
		case 3:
			moved = moveRight();
			break;
		}

		if (moved)
		{
			return;
		}

		OutputDebugString("move failed\n");
	}

}