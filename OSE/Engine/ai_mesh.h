#pragma once
#include <vector>
#include "Utilities/Vector2.h"

// http://theory.stanford.edu/~amitp/GameProgramming/MapRepresentations.html#polygonal-maps

namespace ai {
	
	
	/**
	 * Mesh tile structure.
	 *
	 * @ingroup engine
	 */
	class NavMeshTile
	{
	private:
		std::vector<Vector2> mVerts;     ///< Vertices in tile.
		std::vector<NavMeshTile> mLinks; ///< Adjacent tiles.
	public:
		NavMeshTile() {};
		~NavMeshTile() {};
	};
	
	typedef std::vector<NavMeshTile*> MeshTileList;
	
	/**
	 * Navigation mesh.
	 *
	 * @ingroup engine
	 */
	class NavMesh
	{
	private:
		MeshTileList mTiles; ///< Tiles in Navigation mesh.
		
	public:
		NavMesh();
		~NavMesh();
		
		/**
		 * Add a tile to the NavMesh.
		 *
		 * @param NavMeshTile Tile to add
		 */
		void AddTile(NavMeshTile tile);
		
		
		
	};
	
}