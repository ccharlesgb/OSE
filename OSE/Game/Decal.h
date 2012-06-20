#pragma once
#include "../Engine/BaseObject.h"

class Decal : public BaseObject
{
private:
	std::string mPath;
public:
	static void SetSprite(BaseObject* ent, VariantMap &Data);
	Decal(void);
	~Decal(void);
	void Spawn();
	void Think();
};