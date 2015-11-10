
#include "ADC_Player.h"

namespace ADC
{

	void Player::reset(int lives)
	{
		_hasBeenUpdated = true;
		_lives = lives;
		_coins = 0;
		_pickAxes = 0;
		_swords = 0;
		_keyBronze = 0;
		_keyIron = 0;
		_keyAll = 0;

		World::Position oldPosition = _currentPosition;
		_currentPosition = _startPosition;
		_world->entityEnterCell(*this, oldPosition);
	}
	
	void Player::hittedByMonster(bool& monsterDie)
	{
		if (_swords > 0)
		{
			_swords--;
			_hasBeenUpdated = true;
			monsterDie = true;
			return;
		}

		_looseLifeAndReturnToStart();
	}

	void Player::killMonster()
	{
		_world->entityEnterCell(*this);
		_previousWorldCell = World::Cell::FreeCell;
	}

	bool Player::movePossible(const World::Position& pos, World::Cell cell, bool& consummed)
	{
		consummed = false;

		if (_lives <= 0)
		{
			return false;
		}

		switch (cell)
		{
		case World::Cell::Exit:
			// TODO manage exit reached
			return false;
			break;

		case World::Cell::NaughtyMonster:
			hittedByMonster(consummed);
			if (consummed)
			{
				_world->removeMonsterByPosition(pos);
			}
			return consummed;

		case World::Cell::Trap:
			_looseLifeAndReturnToStart();
			return false;
			break;

		case World::Cell::Coin:
			_coins++;
			_hasBeenUpdated = true;
			consummed = true;
			return true;
			break;

		case World::Cell::Pickaxe:
			_pickAxes++;
			_hasBeenUpdated = true;
			consummed = true;
			return true;
			break;

		case World::Cell::ConcreteWall:
			return false;
			break;

		case World::Cell::BrickWall:
			if (_pickAxes > 0)
			{
				_pickAxes--;
				_hasBeenUpdated = true;
				consummed = true;
				return true;
			}
			return false;
			break;


		case World::Cell::IronDoor:
			if (_keyIron > 0)
			{
				_keyIron--;
				_hasBeenUpdated = true;
				consummed = true;
				return true;
			}
			if (_keyAll > 0)
			{
				_keyAll--;
				_hasBeenUpdated = true;
				consummed = true;
				return true;
			}
			return false;
			break;

		case World::Cell::IronKey:
			_keyIron++;
			_hasBeenUpdated = true;
			consummed = true;
			return true;
			break;

		case World::Cell::BronzeDoor:
			if (_keyBronze > 0)
			{
				_keyBronze--;
				_hasBeenUpdated = true;
				consummed = true;
				return true;
			}
			if (_keyAll > 0)
			{
				_keyAll--;
				_hasBeenUpdated = true;
				consummed = true;
				return true;
			}
			return false;
			break;

		case World::Cell::BronzeKey:
			_keyBronze++;
			_hasBeenUpdated = true;
			consummed = true;
			return true;

		case World::Cell::OpenAllKey:
			_keyAll++;
			_hasBeenUpdated = true;
			consummed = true;
			return true;

		case World::Cell::Sword:
			_swords++;
			_hasBeenUpdated = true;
			consummed = true;
			return true;
			break;
		}

		return true;
	}

	void Player::_looseLifeAndReturnToStart()
	{
		_lives--;
		_hasBeenUpdated = true;
		World::Position oldPosition = _currentPosition;
		_currentPosition = _startPosition;
		_world->entityEnterCell(*this, oldPosition);
	}

}