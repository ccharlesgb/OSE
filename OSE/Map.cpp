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
	int x = 1;
	int y = 1;
	int arr_size = 2;
	//Connor: We should only have one sprite here and draw it multiple times!
	for (int x=-arr_size;x <= arr_size; x++)
	{
		for (int y=-arr_size;y <= arr_size; y++)
		{
			Sprite *s = new Sprite(gGlobals.RenderWindow);
			s->SetTexture("grass1");
			s->SetPosition(Vector2(x * TEXTURE_SIZE, y * TEXTURE_SIZE));
			mSprites.push_back(s);
		}
	}
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
		mSprites[i]->Draw();
	}
}