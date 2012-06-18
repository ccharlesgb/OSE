#pragma once
#include "BasePhysics.h"
#include <iostream>

class Test : public BasePhysics
{
private:
	float mRadius;
	std::string mImage;
	BasePhysics *mSun;
	float mDistance; //Target Distance
	float mVelocity; //Target Tangential Velocity
	float mEcc, mSemiMajor, mSemiMinor;
	float mThetaOffset; //Orientation of axises
public:
	static void SetRadius(BaseObject* ent, VariantMap &Data);
	Test(void);
	~Test(void);
	void Spawn();
	void Think();
	static void SetSun(BaseObject* ent, VariantMap &Data);
	static void SetImage(BaseObject* ent, VariantMap &Data);
};

