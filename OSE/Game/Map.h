#pragma once
#include "../Engine/BaseObject.h"
#include "../Engine/Sprite.h"

#define TEXTURE_SIZE 1024
#define TEXTURE_SCALE 2.f
#define SLOP 5.f //Overlap
#define TEXTURE_SIZE_TRUE TEXTURE_SIZE * TEXTURE_SCALE

class Map : public BaseObject
{
private:
	Sprite* mBackground;
public:
	Map(void);
	~Map(void);
	void Spawn();
	void Think();
	
	void Draw();
};