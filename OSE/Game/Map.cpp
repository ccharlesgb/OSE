#include "Map.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Render/Sprite.h"
#include "../Engine/Profiler.h"


LINKCLASSTONAME("world", Map)

#define ARRAY_SIZE 8

Map::Map(void)
{
	RenderInit();
	SetDrawOrder(RENDERGROUP_BACKGROUND);
	int map_size = ARRAY_SIZE * TEXTURE_SIZE * 2;
	SetRenderBounds(Vector2_Rect(Vector2(map_size, map_size) * -0.5f, Vector2(map_size, map_size)));
	mBackground = new BlendSprite(gGlobals.RenderWindow);
	mBackground->SetTexture1("grass2");
	mBackground->SetTexture2("mud1");
}

Map::~Map(void)
{
	delete mBackground;
}

void Map::Think()
{
	
}

void Map::Spawn()
{

}

void Map::Draw()
{
	Profiler::StartRecord(PROFILE_RENDER_MAP);
	for (int x=-ARRAY_SIZE / 2;x <= ARRAY_SIZE / 2; x++)
	{
		for (int y=-ARRAY_SIZE / 2; y <= ARRAY_SIZE / 2; y++)
		{
			mBackground->SetPosition(Vector2(x * TEXTURE_SIZE, y * TEXTURE_SIZE));
			mBackground->Draw();
		}
	}
	Profiler::StopRecord(PROFILE_RENDER_MAP);
}