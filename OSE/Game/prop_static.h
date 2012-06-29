#pragma once

#include "../Engine/Bases/BasePhysics.h"

class Prop_Static : public BasePhysics
{
private:
	std::string mPath;
public:
	static void SetSprite(BaseObject* ent, VariantMap &Data);
	Prop_Static(void);
	~Prop_Static(void);
	void Spawn();
	void Think();
	void StartTouch(CollisionInfo* info);
	void OnDelete();
	void TakeDamage(const DamageInfo &info);
};

