#pragma once

#include "BaseHuman.h"
#include "../Engine/EntityList.h"
#include "../Engine/Text.h"

class Enemy : public BaseHuman
{
private:
	BaseObject* mTarget;
	double mLastSearch;
	Vector2 mWanderPos;
	double mLastWander;
public:
	Enemy(void);
	~Enemy(void);
	void Spawn();
	void Think();
	void Draw();

	void TakeDamage(const DamageInfo &info);
	void PhysicsSimulate(float delta);
	void StartTouch(CollisionInfo *info);
	void OnDelete();
	void UpdateWanderPos();
};

