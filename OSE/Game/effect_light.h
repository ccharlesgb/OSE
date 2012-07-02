#pragma once

#include "../Engine/Bases/BaseObject.h"
#include "../Engine/Render/LightInfo.h"

class effect_light : public BaseObject
{
private:
	LightInfo mLightInfo;
public:
	effect_light(void);
	~effect_light(void);
	LightInfo* GetLight() {return &mLightInfo;};

	void Think();
};

