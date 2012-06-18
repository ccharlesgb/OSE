#include "Map.h"
#include "Camera.h"
#include "GameGlobals.h"


LINKCLASSTONAME("world", Map)

#define METEOR_SPEED 20

Map::Map(void)
{
	SetPos(Vector2(-TEXTURE_SIZE / 2.f,TEXTURE_SIZE / 2.f));
	SetRenderType(RENDER_CUSTOM);
	SetRenderer(new MapRender);
	InitRenderer();
	GetRenderer()->SetDrawOrder(RENDERGROUP_BACKGROUND);
}

Map::~Map(void)
{

}

void Map::Think()
{

}

void Map::Spawn()
{

}

MapRender::MapRender()
{
	if (!mGrassTex.loadFromFile("images/grass1.png"))
	{
		std::cout << "Invalid filename: " << "grass1" << "\n";
	}
	mSprite.setTexture(mGrassTex);
}

void MapRender::PreDraw(sf::RenderWindow *pRender)
{	

}

void MapRender::Draw(sf::RenderWindow *pRender)
{
	for (int x=-10;x <= 10; x++)
	{
		for (int y=-10;y <= 10; y++)
		{
			mSprite.setPosition(x * TEXTURE_SIZE, y * TEXTURE_SIZE);
			pRender->draw(mSprite);
		}
	}
}

MapRender::~MapRender()
{

}