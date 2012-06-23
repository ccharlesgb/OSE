#pragma once

#include "../Engine/Bases/BasePhysics.h"

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
	void StartTouch(CollisionInfo* info);
	void OnDelete();
	void TakeDamage(const DamageInfo &info);
};

