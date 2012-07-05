#pragma once
#include "Utilities/Vector2.h"

namespace ai {
	
	/**
	 * Mesh tile structure.
	 *
	 * @ingroup engine
	 */
	struct NavMeshTile
	{
		Vector2 *verts;     ///< Vertices in tile.
		NavMeshTile *links; ///< Adjacent tiles.
	};
	
	/**
	 * Navigation mesh.
	 *
	 * @ingroup engine
	 */
	class NavMesh
	{
	private:
		NavMeshTile *tiles; ///< Tiles in Navigation mesh.
		
	public:
		
	};
	
}