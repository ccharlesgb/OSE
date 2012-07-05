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
		std::vector<Vector2> mVertices;     ///< Vertices in tile.
		std::vector<NavMeshTile> mLinks; ///< Adjacent tiles.
	public:
		NavMeshTile();
		~NavMeshTile();
		
		/**
		 * Add a verticle to the mesh tile.
		 *
		 * @param Vector2 Verticle to add.
		 */
		void AddVerticies(const Vector2 vert);
		
		/**
		 * Get all the verticies for the tile.
		 *
		 * @return std::vector<Vector2> Vector of vertices.
		 */
		std::vector<Vector2> GetVertices() { return mVertices; };
	};
	
	typedef std::vector<NavMeshTile> MeshTileList;
	
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
		void AddTile(const NavMeshTile tile);
		
		/**
		 * Draw the mesh, useful for debugging.
		 */
		void DrawDebug();
		
	};
	
}