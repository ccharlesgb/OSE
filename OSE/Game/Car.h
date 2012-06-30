#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/Render/Line.h"

class Car : public BasePhysics
{
private:
	BaseObject* mDriver;
	bool InUse() { return mDriver != NULL; };
	float mWheelAngle;
	float mThrottle;
	float mEnterTime;
	
	bool mFrontWheelSkid;
	bool mBackWheelSkid;
	float mFrontWheelTraction;
	float mBackWheelTraction;

	Sprite* mWheelSprite;
	Line* mLine;
	Line* mLine2;
	Line* mLine3;
	Line* mLine4;
	double mLastTrailDrop;
public:
	Car(void);
	~Car(void);
	void Spawn();
	void OnDelete();
	void Think();
	void StartTouch(CollisionInfo* info);
	void Exit(Vector2 position);
	void Use(BaseObject *ply);
	void Draw();
	void PhysicsSimulate(float delta);
};
