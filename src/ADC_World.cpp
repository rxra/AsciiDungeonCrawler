
#include "ADC_World.h"
#include "ADC_Player.h"
#include "ADC_Monster.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace ADC
{

	const set<World::Cell> World::s_KnownCells = {
		World::Cell::Start,
		World::Cell::Exit,
		World::Cell::Player,
		World::Cell::Trap,
		World::Cell::Coin,
		World::Cell::Pickaxe,
		World::Cell::ConcreteWall,
		World::Cell::BrickWall,
		World::Cell::NaughtyMonster,
		World::Cell::IronDoor,
		World::Cell::IronKey,
		World::Cell::BronzeDoor,
		World::Cell::BronzeKey,
		World::Cell::OpenAllKey,
		World::Cell::Sword,
		World::Cell::FreeCell
	};

	shared_ptr<World> World::Create(std::string& filename, int playerLives)
	{
		// create the world as a shared pointer
		auto world = make_shared<World>();
		vector<Position> monstersPositions;
		if (!world->_load(filename,monstersPositions))
		{
			// if we failed to load the map returns an empty (null) shared pointer
			return shared_ptr<World>();
		}

		// if the world has been loaded

		// create the player
		// using the start cell
		world->_player = make_shared<Player>(world, world->_startCell, playerLives);

		for (auto mp : monstersPositions)
		{
			world->_monsters.push_back(make_shared<Monster>(world, mp, 5));
		}

		return world;
	}

	World::World()
		: CanBeUpdated(true), _width(0), _height(0), _startCell({ -1, -1 }), _exitCell({ -1, -1 })
	{
	}

	World::~World()
	{
	}

	void World::reset(std::string& filename, int playerLives)
	{
		_map.clear();
		_monsters.clear();

		vector<Position> monstersPositions;
		_load(filename,monstersPositions);
		
		_player->reset(playerLives);

		_hasBeenUpdated = true;
	}

	void World::update()
	{
		vector<shared_ptr<Monster>> monstersToRemove;

		for (auto m : _monsters)
		{
			if (!m->update())
			{
				monstersToRemove.push_back(m);
			}
		}

		for (auto m : monstersToRemove)
		{
			_monsters.remove(m);
		}
	}

	void World::entityEnterCell(Entity& entity)
	{
		entity.setPreviousWorldCell(_map[entity.getPosition().x + entity.getPosition().y * _width]);
		_map[entity.getPosition().x + entity.getPosition().y * _width] = entity.getCell();
		_hasBeenUpdated = true;
	}

	void World::entityEnterCell(Entity& entity, Position previousPosition)
	{
		_map[previousPosition.x + previousPosition.y * _width] = entity.getPreviousWorldCell();
		entityEnterCell(entity);
	}

	bool World::isCellFree(size_t x, size_t y) const
	{
		if (x < 0 || x >= _width)
		{
			return false;
		}

		if (y < 0 || y >= _height)
		{
			return false;
		}

		return _map[x + y * _width] != Cell::ConcreteWall;
	}

	bool World::_load(const string& filename, std::vector<Position>& monsterPositions)
	{
		ifstream myfile(filename);
		if (!myfile.is_open()) {
			cerr << "Cannot open world file: " << filename << endl;
			return false;
		}

		// first read to check the map file is ok and to compute width and height
		string line;
		while (getline(myfile, line))
		{
			// check world width

			// world width not yet initialzed
			// world width is intialized with the width of the first line
			if (_width == 0)
			{
				_width = line.length();
			}
			// line length is smaller than world width ... display a warning
			else if (line.length() < _width)
			{
				cout << "Line is smaller than world width(" << _width << "): " << line << endl;
			}
			// line length is greater than world width ... not support so display an error and leave (returning false)
			else if (line.length() > _width)
			{
				cerr << "Line is greater than world width(" << _width << "): " << line << endl;
				// set _width/_height to 0 to invalidate the world
				_width = _height = 0;
				return false;
			}

			int x = 0;
			for (auto c : line)
			{
				if (s_KnownCells.find(static_cast<World::Cell>(c)) == s_KnownCells.end())
				{
					cerr << "World file (" << filename << ") contains invalid cell: " << c << " " << static_cast<int>(c) << " (" << x << " x " << _height << ')' << endl;
					// set _width/_height to 0 to invalidate the world
					_width = _height = 0;
					return false;
				}
				x++;
			}

			_height++;
		}

		// initialize map
		_map.resize(_width*_height);

		// restart reading map file to fill _map
		myfile.clear();
		myfile.seekg(0, ios::beg);

		int x = 0;
		int y = 0;
		while (getline(myfile, line))
		{
			x = 0;
			for (auto c : line)
			{
				Cell cell = static_cast<Cell>(c);
				switch (cell)
				{
				case Cell::Start:
					// TODO: possibly add a warning if _startCell already initialized
					// to prevent having more than one start in the file
					// (test x != -1 && y != -1
					_startCell.x = x;
					_startCell.y = y;
					break;

				case Cell::Exit:
					// TODO: possibly add a warning if _exitCell already initialized
					// to prevent having more than one end in the file
					// (test x != -1 && y != -1
					_exitCell.x = x;
					_exitCell.y = y;
					break;

				case Cell::NaughtyMonster:
					monsterPositions.push_back({ x, y });
					// change cell to FreeCell so monster previousWorldCell will be corretly initiliazed,
					// so cell is freed when monster leave the cell.
					cell = Cell::FreeCell;
					break;
				}

				_map[(x++) + y * _width] = cell;
			}
			y++;
		}

		if (_startCell.x == -1 || _startCell.y == -1)
		{
			cerr << "Start cell is not defined in the map file" << endl;
			return false;
		}

		if (_exitCell.x == -1 || _exitCell.y == -1)
		{
			cerr << "Exit cell is not defined in the map file" << endl;
			return false;
		}

		return true;
	}

}