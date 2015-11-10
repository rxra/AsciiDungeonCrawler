#if !defined(ADC_WORLD_H)
#define ADC_WORLD_H

#include <string>
#include <set>
#include <vector>
#include <list>
#include <memory>

#include "ADC_CanBeUpdated.h"

namespace ADC
{

	// forward declaration
	class Entity;
	class Player;
	class Monster;

	class World : public CanBeUpdated
	{
	public:

		/**
		  * Defines all cell's types available in the maze.
		  */
		// Remarks: if Cell is updated you need to update s_KnownCells (see bellow if this file and in ADC_World.cpp)
		enum class Cell
		{
			None = -1,
			Start = 'S', /** Cell where the player starts. */
			Exit = 'A', /** Cell the player need to reach to exist the maze. */
			Player = ':', /** The player is self. */
			Trap = '@', /** A trap. If encounter the player loose a life and go back to start. */
			Coin = '$', /** A coin. */
			Pickaxe = ')', /** A pickaxe. Can be used to break brick wall. */
			ConcreteWall = '#', /** ConcreteWall. Cannot be broke. */
			BrickWall = 'H', /** BrickWall. Can be breaked using a pickaxe. */
			NaughtyMonster = 'O', /** A monster. */
			IronDoor = 'F', /** An iron door. Need an iron key or an "open all" key to be opened. */
			IronKey = 'f', /** An iron key. */
			BronzeDoor = 'B', /** A bronze door. Need a broze key or an "open all" key to be opened. */
			BronzeKey = 'b', /** A bronze key. */
			OpenAllKey = 'p', /** A key which can open all doors. */
			Sword = 'e', /** A swork. */
			FreeCell = ' '
		};

		/**
		  * Describe a position in the maze.
		  * 0x0 is the top left corner of the maze. And (width-1)x(height-1) is the bottom right corner of the maze.
		  * \see getWidth, getHeight.
		  */
		struct Position
		{
			int x;
			int y;

			int sqrDistance(const Position& p) const;

			bool operator==(const Position& p) const;

		};

		/**
		  * Create a world from a map file.
		  * You must use it instead of calling the World() constructor.
		  *
		  * \code
		  * std::string filename;
		  * // get the filename for the command line for exemple
		  * auto world = ADC::World::Create(filename);
		  * \endcode
		  *
		  * \return
		  */
		static std::shared_ptr<World> Create(std::string& filename, int playerLives);

		/**
		  * Default constructor. 
		  * Do not use it. To create a world prefere ADC::World::Create.
		  * \see Create.
		  */
		World();

		/**
		  * Destructor. Release internal resources.
		  */
		~World();

		void reset(std::string& filename, int playerLives);

		void update();

		/** Width of the maze. */
		size_t getWidth() const;

		/** Height of the maze. */
		size_t getHeight() const;

		/** Given a position in the maze returns the cell type. */
		Cell getCell(const Position& p) const;

		/** Given a position in the maze returns the cell type. */
		Cell getCell(size_t x, size_t y) const;

		/** Update the maze with the current entity. Set the maze cell using the entity position.
		  * Remarks: Entity previous world cell is updated.
		  */
		void entityEnterCell(Entity& entity);
		void entityEnterCell(Entity& entity, Position previousPosition);

		/** Returns true is the cell is in the maze and is not a ConcreteWall.
		  * Then you might need to check the cell type (using getCell).
		  * \see getCell.
		  */
		bool isCellFree(const Position& p) const;

		/** Returns true is the cell is in the maze and is not a ConcreteWall.
		* Then you might need to check the cell type (using getCell).
		* \see getCell.
		*/
		bool isCellFree(size_t x, size_t y) const;

		/** Returns the player (which is unique). */
		std::shared_ptr<Player>& getPlayer();

		void removeMonsterByPosition(const Position& position);

	private:

		////////////////////////////////////////////////////////////////////////////////
		// private constructor/operator not impletemend to be sure we cannot call them.
		World(const World& world) = delete;
		World(const World&& world) = delete;
		World& operator=(const World& world) = delete;
		World& operator=(const World&& world) = delete;
		////////////////////////////////////////////////////////////////////////////////

		bool _load(const std::string& filename, std::vector<Position>& monsterPositions);

		static const std::set<Cell> s_KnownCells;

		size_t _width;
		size_t _height;
		Position _startCell;
		Position _exitCell;
		std::shared_ptr<Player> _player;
		std::vector<Cell> _map;
		std::list<std::shared_ptr<Monster>> _monsters;

	};

}


////////////////////////////////////////////////////////////////////////////////
// Inline implementations


inline size_t ADC::World::getWidth() const
{
	return _width;
}

inline size_t ADC::World::getHeight() const
{
	return _height;
}

inline ADC::World::Cell ADC::World::getCell(const Position& p) const
{
	return _map[p.x + p.y * _width];
}

inline ADC::World::Cell ADC::World::getCell(size_t x, size_t y) const
{
	return _map[x + y * _width];
}

inline bool ADC::World::isCellFree(const Position& p) const
{
	return isCellFree(p.x, p.y);
}

inline std::shared_ptr<ADC::Player>& ADC::World::getPlayer()
{
	return _player;
}

inline int ADC::World::Position::sqrDistance(const Position& p) const
{
	int xd = p.x - x;
	int xy = p.y - y;

	return (xd*xd + xy*xy);
}

inline bool ADC::World::Position::operator==(const Position& p) const
{
	return x == p.x && y == p.y;
}

#endif // ADC_WORLD_H