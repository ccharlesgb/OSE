#include "ai_mesh.h"
#include "GameGlobals.h"

namespace ai {
	
	/**
	 * NavMeshTile
	 */
	NavMeshTile::NavMeshTile()
	{
		
	};
	
	NavMeshTile::~NavMeshTile()
	{
		
	};
	
	void NavMeshTile::AddVerticies(const Vector2 vert)
	{
		mVertices.push_back(vert);
	};
	
	
	/**
	 * NavMesh
	 */
	NavMesh::NavMesh()
	{
		
	};
	
	NavMesh::~NavMesh()
	{
		
	};
	
	void NavMesh::AddTile(const NavMeshTile tile)
	{
		mTiles.push_back(tile);
	}
	
	void NavMesh::DrawDebug()
	{
		MeshTileList::iterator it;
		
		for (it = mTiles.begin(); it != mTiles.end(); it++)
		{
			NavMeshTile tile = *it;
			std::vector<Vector2> vertices = tile.GetVertices();
			
			int vertexCount = vertices.size();
			
			sf::ConvexShape Shape(vertexCount); 
			Shape.setOutlineThickness(1.f); 
			Shape.setOutlineColor(sf::Color::Cyan);
			Shape.setFillColor(sf::Color::Transparent);
			for (int32 i = 0; i < vertexCount; ++i) 
			{
				Shape.setPoint(i, vertices[i].SF());
			}
			gGlobals.RenderWindow->draw(Shape);
			
			// TODO: Draw Relations between Tiles.
		}
	}
}