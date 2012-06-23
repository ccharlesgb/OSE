#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/Render/Line.h"

class Ship : public BasePhysics
{
private:
	BaseObject* mDriver;
	bool InUse() { return mDriver != NULL; };
	float mWheelAngle;
	float mThrottle;
	float mEnterTime;
	Sprite* mWheelSprite;
	Line* mLine;
public:
	Ship(void);
	~Ship(void);
	void Spawn();
	void OnDelete();
	void Think();
	void StartTouch(CollisionInfo* info);
	void Exit(Vector2 position);
	void Use(BaseObject *ply);
	void Draw();
};
