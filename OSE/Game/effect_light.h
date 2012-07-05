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
	
	//Redirects to light info
	void SetSpreadAngle(float ang) {mLightInfo.SetSpreadAngle(ang);};
	float GetSpreaAngle() {return mLightInfo.GetSpreadAngle();};
	void SetLightColour(Colour col) {mLightInfo.SetColour(col);};
	Colour GetLightColour()
	{
		Colour col;
		sf::Color col_sf = mLightInfo.GetColour();
		col.r = col_sf.r;
		col.g = col_sf.g;
		col.b = col_sf.b;
		col.a = col_sf.a;
		return col;
	}
};

