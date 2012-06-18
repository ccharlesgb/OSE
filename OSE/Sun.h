#pragma once
#include "basephysics.h"

class Sun : public BasePhysics
{
private:
	std::vector<BaseObject*> mPlanets;
	float mRadius;
public:
	Sun(void);
	~Sun(void);

	void Spawn();
	void Think();
	void CreatePlanet(float size, float distance);
};

