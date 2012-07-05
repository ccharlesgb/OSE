#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/Render/Line.h"

class effect_light;

class Car : public BasePhysics
{
private:
	BaseHuman* mDriver;
	bool InUse() { return mDriver != NULL; };
	float mWheelAngle;
	float mThrottle;
	float mEnterTime;
	
	bool mFrontWheelSkid;
	bool mBackWheelSkid;
	float mFrontWheelTraction;
	float mBackWheelTraction;

	effect_light *mLight;

	Sprite* mWheelSprite;
	double mLastTrailDrop;
public:
	Car(void);
	~Car(void);
	void Spawn();
	void OnDelete();
	void Think();
	void StartTouch(CollisionInfo* info);
	void Exit(Vector2 position);
	void Use(BaseHuman *ply);
	void Draw();
	void PhysicsSimulate(float delta);
};
