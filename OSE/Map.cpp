#include "Map.h"
#include "Camera.h"
#include "GameGlobals.h"
#include "Sprite.h"


LINKCLASSTONAME("world", Map)

#define METEOR_SPEED 20

Map::Map(void)
{
	RenderInit();
	SetDrawOrder(RENDERGROUP_BACKGROUND);
	
	mSprite = new Sprite(gGlobals.RenderWindow);
	mSprite->SetTexture("grass1");
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
	int arr_size = 2;
	for (int x=-arr_size;x <= arr_size; x++)
	{
		for (int y=-arr_size;y <= arr_size; y++)
		{
			mSprite->SetPosition(Vector2(x * TEXTURE_SIZE, y * TEXTURE_SIZE));
			mSprite->Draw();
		}
	}
}