#if !defined(ADC_PLAYER_H)
#define ADC_PLAYER_H

#include "ADC_Entity.h"

namespace ADC
{

	class Player : public Entity, public CanBeUpdated
	{
	public:

		Player(std::shared_ptr<World>& world, World::Position& start, int lives);

		virtual ~Player() {}

		virtual bool movePossible(World::Cell cell, bool &consummed);

		void reset(int lives);

		void hittedByMonster(bool &monsterDie);
		void killMonster();

		int getLives() const;
		int getCoins() const;
		int getPickAxes() const;
		int getSwords() const;
		int getKeyBronze() const;
		int getKeyIron() const;
		int getKeyAll() const;

	private:

		void _looseLifeAndReturnToStart();

		const World::Position _startPosition;
		int _lives;
		int _coins;
		int _pickAxes;
		int _swords;
		int _keyBronze;
		int _keyIron;
		int _keyAll;

	};

}

inline ADC::Player::Player(std::shared_ptr<World>& world, World::Position& start, int lives)
	: ADC::Entity(world, World::Cell::Player, start), CanBeUpdated(true),
	_startPosition(start),
	_lives(lives),
	_coins(0),
	_pickAxes(0),
	_swords(0),
	_keyBronze(0),
	_keyIron(0),
	_keyAll(0)
{
}

inline int ADC::Player::getLives() const
{
	return _lives;
}

inline int ADC::Player::getCoins() const
{
	return _coins;
}

inline int ADC::Player::getPickAxes() const
{
	return _pickAxes;
}

inline int ADC::Player::getSwords() const
{
	return _swords;
}

inline int ADC::Player::getKeyBronze() const
{
	return _keyBronze;
}

inline int ADC::Player::getKeyIron() const
{
	return _keyIron;
}

inline int ADC::Player::getKeyAll() const
{
	return _keyAll;
}

#endif // ADC_PLAYER_H