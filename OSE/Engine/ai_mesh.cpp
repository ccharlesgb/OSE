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
	
	void NavMeshTile::AddLink(NavMeshTile *tile)
	{
		mLinks.push_back(tile);
	};
	
	Vector2 NavMeshTile::GetCenter()
	{
		float x = 0;
		float y = 0;
		
		for (int i = 0; i < mVertices.size(); i++)
		{
			x += mVertices[i].x;
			y += mVertices[i].y;
		}
		
		x /= mVertices.size();
		y /= mVertices.size();
		
		return Vector2(x, y);
	}
	
	
	/**
	 * NavMesh
	 */
	NavMesh::NavMesh()
	{
		int size = 2;
		int tile_size = 100;
		
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				int x_pos = x * tile_size;
				int y_pos = y * tile_size;
				
				ai::NavMeshTile tile = *new ai::NavMeshTile();
				tile.AddVerticies(Vector2(x_pos,       y_pos));
				tile.AddVerticies(Vector2(x_pos,       y_pos + 100));
				tile.AddVerticies(Vector2(x_pos + 100, y_pos + 100));
				tile.AddVerticies(Vector2(x_pos + 100, y_pos));
				this->AddTile(tile);
			}
		}
	};
	
	NavMesh::~NavMesh()
	{
		
	};
	
	void NavMesh::AddTile(NavMeshTile tile)
	{
		MeshTileList referenceTiles;
		
		// Get all tiles that shares a vertex with the tile.
		std::vector<Vector2> vertices = tile.GetVertices();
		for (int i = 0; i < vertices.size(); ++i)
		{
			MeshTileList tmp = GetTilesWithVertex(vertices[i]);
			referenceTiles.insert(referenceTiles.end(), tmp.begin(), tmp.end());
		}
		
		// Link the tiles.
		MeshTileList::iterator it;
		for (it = referenceTiles.begin(); it != referenceTiles.end(); it++)
		{
			NavMeshTile *t = *it;
			
			t->AddLink(&tile);
			tile.AddLink(t);
		}
		
		mTiles.push_back(&tile);
	}
	
	void NavMesh::DrawDebug()
	{
		MeshTileList::iterator it;
		for (it = mTiles.begin(); it != mTiles.end(); it++)
		{
			NavMeshTile *tile = *it;
			std::vector<Vector2> vertices = tile->GetVertices();
			
			int vertexCount = vertices.size();
			
			sf::ConvexShape Shape(vertexCount); 
			Shape.setOutlineThickness(1.f); 
			Shape.setOutlineColor(sf::Color::Cyan);
			Shape.setFillColor(sf::Color::Transparent);
			for (int32 i = 0; i < vertexCount; ++i) 
			{
				Shape.setPoint(i, ig::GameToSFML(vertices[i]).SF());
			}
			gGlobals.RenderWindow->draw(Shape);
			
			MeshTileList::iterator links_it;
			MeshTileList links = tile->GetLinks();
			
			for (links_it = links.begin(); links_it != links.end(); links_it++)
			{
				NavMeshTile *t = *links_it;
				
				sf::VertexArray lines(sf::LinesStrip, 2);
				lines[0].position = ig::GameToSFML(tile->GetCenter()).SF();
				lines[1].position = ig::GameToSFML(t->GetCenter()).SF();
				
				gGlobals.RenderWindow->draw(lines);
			}
			/*
			sf::VertexArray lines(sf::LinesStrip, 2);
			lines[0].position = pos1.SF();
			lines[1].position = pos2.SF();
			pRender->draw(lines);
			*/
			// TODO: Draw Relations between Tiles.
		}
	}
	
	/**
	 * Loop all tiles inside the NavMesh and check if any have a vertex in the position.
	 */
	MeshTileList NavMesh::GetTilesWithVertex(Vector2 vert)
	{
		MeshTileList tiles;
		
		MeshTileList::iterator it;
		for (it = mTiles.begin(); it != mTiles.end(); it++)
		{
			NavMeshTile *tile = *it;
			std::vector<Vector2> vertices = tile->GetVertices();
			for (int i = 0; i < vertices.size(); i++)
			{
				if (vert == vertices[i])
				{
					tiles.push_back(tile);
					break;
				}
			}
		}
		
		return tiles;
	}
}