#pragma once

#include "../Engine/Bases/BaseObject.h"

class Decal : public BaseObject
{
private:
	float mDietime;
public:
	static void SetDietime(BaseObject* ent, VariantMap &Data);
	Decal(void);
	~Decal(void);
	void Spawn();
	void Think();
};