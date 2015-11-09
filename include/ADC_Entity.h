#if !defined(ADC_ENTITY_H)
#define ADC_ENTITY_H

#include "ADC_World.h"

namespace ADC
{

	class Entity
	{
	public:

		Entity(std::shared_ptr<World>& world, World::Cell cell, World::Position& start);

		virtual ~Entity() {}

		World::Cell getCell() const;
		World::Cell getPreviousWorldCell() const;

		void setPreviousWorldCell(World::Cell cell);

		const World::Position& getPosition() const;

		bool moveUp();
		bool moveDown();
		bool moveLeft();
		bool moveRight();

		virtual bool movePossible(World::Cell cell, bool& consummed) = 0;


	protected:

		std::shared_ptr<World> _world;
		World::Cell _cell;
		World::Position _currentPosition;
		World::Cell _previousWorldCell;

	private:

		bool move(const World::Position& newPosition);

	};

}

inline ADC::World::Cell ADC::Entity::getCell() const
{
	return _cell;
}

inline ADC::World::Cell ADC::Entity::getPreviousWorldCell() const
{
	return _previousWorldCell;
}

inline void ADC::Entity::setPreviousWorldCell(World::Cell cell)
{
	_previousWorldCell = cell;
}

inline const ADC::World::Position& ADC::Entity::getPosition() const
{
	return _currentPosition;
}

#endif // ADC_ENTITY_H