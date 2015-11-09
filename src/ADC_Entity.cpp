
#include "ADC_Entity.h"

namespace ADC
{

	Entity::Entity(std::shared_ptr<World>& world, World::Cell cell, ADC::World::Position& start)
		: _world(world), _cell(cell), _currentPosition(start), _previousWorldCell(World::Cell::None)
	{
		_world->entityEnterCell(*this);
	}

	bool Entity::moveUp()
	{
		return move({ _currentPosition.x, _currentPosition.y - 1 });
	}

	bool Entity::moveDown()
	{
		return move({ _currentPosition.x, _currentPosition.y + 1 });
	}

	bool Entity::moveLeft()
	{
		return move({ _currentPosition.x - 1, _currentPosition.y });
	}

	bool Entity::moveRight()
	{
		return move({ _currentPosition.x + 1, _currentPosition.y });
	}

	bool Entity::move(const World::Position& newPosition)
	{
		bool consummed = false;
		if (!_world->isCellFree(newPosition) || !movePossible(_world->getCell(newPosition),consummed))
		{
			return false;
		}

		World::Position oldPosition = _currentPosition;
		_currentPosition = newPosition;
		_world->entityEnterCell(*this, oldPosition);
		if (consummed)
		{
			_previousWorldCell = World::Cell::FreeCell;
		}

		return true;
	}


}