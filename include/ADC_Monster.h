#if !defined(ADC_MONSTER_H)
#define ADC_MONSTER_H

#include "ADC_Entity.h"

namespace ADC
{

	class Monster : public Entity
	{
	public:

		Monster(std::shared_ptr<World>& world, World::Position& start, int distanceToPlayer);

		virtual ~Monster() {}

		virtual bool movePossible(World::Cell cell, bool& consummed);

		bool update();

	private:

		void _getCloserToPlayer();
		void _randomMove();

		int _sqrDistanceToPlayer;

	};

}

inline ADC::Monster::Monster(std::shared_ptr<World>& world, World::Position& start, int distanceToPlayer)
	: ADC::Entity(world, World::Cell::NaughtyMonster, start), _sqrDistanceToPlayer(distanceToPlayer*distanceToPlayer)
{
}


#endif // ADC_MONSTER_H