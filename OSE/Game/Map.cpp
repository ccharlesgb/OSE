#include "Map.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Render/Sprite.h"
#include "../Engine/Profiler.h"


LINKCLASSTONAME("world", Map)

Map::Map(void)
{
	RenderInit();
	SetDrawOrder(RENDERGROUP_BACKGROUND);
	
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
	int arr_size = 70;
	for (int x=-arr_size;x <= arr_size; x++)
	{
		for (int y=-arr_size;y <= arr_size; y++)
		{
			mBackground->SetPosition(Vector2(x * TEXTURE_SIZE, y * TEXTURE_SIZE));
			mBackground->Draw();
		}
	}
	Profiler::StopRecord(PROFILE_RENDER_MAP);
}