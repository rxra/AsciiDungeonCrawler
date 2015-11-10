
#include "ADC_Monster.h"
#include "ADC_Player.h"
#include "ADC_RandomGenerator.h"

using namespace std;

namespace ADC
{

	bool Monster::movePossible(const World::Position& pos, World::Cell cell, bool& consummed)
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
		switch (RandomGenerator::RandomDirection()) {
		case World::Direction::Up:
			moveUp();
			break;

		case World::Direction::Down:
			moveDown();
			break;

		case World::Direction::Left:
			moveLeft();
			break;

		case World::Direction::Right:
			moveRight();
			break;
		}
	}

}