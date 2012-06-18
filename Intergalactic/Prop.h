#pragma once
#include "BasePhysics.h"
#include <iostream>

class Prop : public BasePhysics
{
private:
	std::string mPath;
public:
	static void SetSprite(BaseObject* ent, VariantMap &Data);
	Prop(void);
	~Prop(void);
	void Spawn();
	void Think();
};

