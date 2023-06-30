#pragma once
#include "FlatBuffer.h"
#include "Vec.h"

/*
This is taking lots of design inspiration from an engine shown
in this video: https://www.youtube.com/watch?v=ALL7AQ1MRas
github repo: https://github.com/eduard-permyakov/permafrost-engine/blob/master/src/navigation/public/nav.h

Our navmesh will consist of a grid of chunks, each holding a grid
of tiles. Each chunk consisting of it's own "border tiles" near
all of it's edges to dictate which tiles are passable from one
chunk to it's neighboring ones.

Assuming a 3x3 set of chunks each 5x5 tiles large you'd have
a border like...

O tile
# border tile

OOOOOOOOOOOOOOO
OOOO##OOO##OOOO
OOOO##OOO##OOOO
OOOO##OOO##OOOO
O#############O
O#############O
OOOO##OOO##OOOO
OOOO##OOO##OOOO
OOOO##OOO##OOOO
O#############O
O#############O
OOOO##OOO##OOOO
OOOO##OOO##OOOO
OOOO##OOO##OOOO
OOOOOOOOOOOOOOO
*/

typedef uint16_t PathfindingSubjectID;
typedef uint32_t PathfindingWaypointID;

template<uint32_t width_count, uint32_t height_count, uint32_t size_per_tile, 
	uint32_t max_entities_per_tile = 16, uint8_t size_per_chunk = 32, uint8_t max_layers = 3, uint8_t max_waypoints = 1000>
class Pathfinding {
	#define chunk_width_count (width_count / size_per_chunk)
	#define chunk_height_count (height_count / size_per_chunk)

	enum EDGE_STATE {
		EDGE_STATE_ACTIVE,
		EDGE_STATE_BLOCKED
	};
	enum DIRECTION {
		DIRECTION_NULL,
		DIRECTION_UP,
		DIRECTION_RIGHT,
		DIRECTION_DOWN,
		DIRECTION_LEFT
	};

	class Tile {
		FlatBuffer<PathfindingSubjectID, max_entities_per_tile> IDs;
	public:
		__forceinline void pushIfUnique(PathfindingSubjectID id) {
			uint32_t count = IDs.count;
			for (uint32_t i = 0; i < count; i++) {
				if (IDs[i] == id) {
					std::cout << "Error: Tile::pushIfUnique()'s id param already exists in tile" << std::endl;
					return;
				}
			}
			IDs.push(id);
		}
		__forceinline void remove(PathfindingSubjectID id) {
			uint32_t count = IDs.count;
			for (uint32_t i = 0; i < count; i++) {
				if (IDs[i] == id) {
					IDs.count--;
					IDs[i] = IDs[count - 1];
					return;
				}
			}
			std::cout << "Error: Tile::remove()'s id param not present in tile" << std::endl;
		}
		__forceinline uint32_t isBlocked() {
			return IDs.count;
		}
		__forceinline uint32_t getCount() {
			return IDs.count;
		}
	};
	struct Waypoint {
		FlatBuffer<uint8_t, width_count * height_count> flowFields;
		Vec2D<uint16_t> start, end;

	};
	struct Chunk {
		bool isPassable[size_per_chunk * 4 - 4];
		bool cornerIsPassable[4];
		FlatBuffer<Vec2D<uint8_t>, size_per_chunk * 4> validPaths;
	};
	struct Layer {

		FlatBuffer<Tile, width_count * height_count> tiles;
		FlatBuffer<Waypoint, max_waypoints> waypoints;
		FlatBuffer<Chunk, chunk_width_count * chunk_height_count> chunks;
		__forceinline void placeBlocker(PathfindingSubjectID id, const Vec2D<uint32_t>& pos, const Vec2D<uint32_t>& siz) {
			Vec2D<uint32_t> start, end;
			start = pos; start /= size_per_tile;
			end = pos; end += siz; end /= size_per_tile;
			for (uint32_t x = start.x; x < end.x; x++) {
				for (uint32_t y = start.y; y < end.y; y++) {
					if (tiles[x + y * width_count].getCount() == max_entities_per_tile) {
						std::cout << "Error: PathFinding::Layer::place() failed, tile has too many members" << std::endl;
						throw;
					}
					tiles[x + y * width_count].pushIfUnique(id);
				}
			}
		}
		void build() {

		}
	private:
		void buildChunk(uint32_t chunkIndex) {
			Chunk& chunk = chunks[chunkIndex];
			Vec2D<uint16_t> chunkStart;
			Vec2D<uint16_t> chunkEnd = chunkStart;
		}
	};
	FlatBuffer<Layer, max_layers> layers;
	PathfindingSubjectID lastSubject;
public:
	__forceinline void init() {
		lastSubject = 0;  // first subject starts at 1 

		if (width_count % size_per_chunk || height_count % size_per_chunk) {
			std::cout << "Pathfinding::init() detects that dimensions aren't divisible by size_per_chunk" << std::endl;
			system("pause");
			exit(0);
		}
	}
	__forceinline PathfindingSubjectID subscribeBlocker(uint8_t layerID, const Vec2D<uint32_t>& pos, const Vec2D<uint32_t>& siz) {
		lastSubject++;
		layers[layerID].placeBlocker(lastSubject, pos, siz);
		return lastSubject;
	}
};