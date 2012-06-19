#include "Map.h"
#include "Camera.h"
#include "GameGlobals.h"
#include "Sprite.h"


LINKCLASSTONAME("world", Map)

#define METEOR_SPEED 20

Map::Map(void)
{
	RenderInit();
	int x = 1;
	int y = 1;
	//for (int x=-10;x <= 10; x++)
	//{
	//	for (int y=-10;y <= 10; y++)
	//	{
			Sprite s = *new Sprite(gGlobals.RenderWindow);
			s.SetTexture("grass1");
			s.SetPosition(Vector2(x * TEXTURE_SIZE, y * TEXTURE_SIZE));
			mSprites.push_back(s);
	//	}
	//}
	
	/*
	SetPos(Vector2(-TEXTURE_SIZE / 2.f,TEXTURE_SIZE / 2.f));
	SetRenderType(RENDER_CUSTOM);
	SetRenderer(new MapRender);
	InitRenderer();
	GetRenderer()->SetDrawOrder(RENDERGROUP_BACKGROUND);
	*/
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

void Map::Draw()
{
	for(unsigned int i = 0; i < mSprites.size(); i++)
	{
		mSprites[i].Draw();
	}
}

/*

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
*/