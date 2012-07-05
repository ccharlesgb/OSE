#pragma once
#include <vector>
#include "Utilities/Vector2.h"

// http://theory.stanford.edu/~amitp/GameProgramming/MapRepresentations.html#polygonal-maps

namespace ai {
	
	class NavMeshTile;
	
	typedef std::vector<NavMeshTile> MeshTileList;
	
	/**
	 * Mesh tile structure.
	 *
	 * @ingroup engine
	 */
	class NavMeshTile
	{
	private:
		std::vector<Vector2> mVertices;     ///< Vertices in tile.
		MeshTileList mLinks; ///< Adjacent tiles.
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
		
		/**
		 * Add a link.
		 *
		 * @param NavMeshTile Tile to link to.
		 */
		void AddLink(const NavMeshTile tile);
		
		MeshTileList GetLinks() { return mLinks; };
		
		Vector2 GetCenter();
	};
	
	/**
	 * Navigation mesh.
	 *
	 * @ingroup engine
	 */
	class NavMesh
	{
	private:
		MeshTileList mTiles; ///< Tiles in Navigation mesh.
		
		/**
		 * Return all tiles that has a vertex at position vert.
		 *
		 * @param Vector2 Position to check.
		 * @return MeshTileList List of tiles with vertex at position.
		 */
		MeshTileList GetTilesWithVertex(Vector2 vert);
		
	public:
		NavMesh();
		~NavMesh();
		
		/**
		 * Add a tile to the NavMesh.
		 *
		 * @param NavMeshTile Tile to add
		 */
		void AddTile(NavMeshTile tile);
		
		/**
		 * Draw the mesh, useful for debugging.
		 */
		void DrawDebug();
		
	};
	
}