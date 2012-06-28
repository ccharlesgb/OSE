#pragma once

#include "../Engine/Render/BaseHUD.h"
#include "../Engine/Text.h"

class HUD : public BaseHUD
{
private:
	Text* mHealth;
	Text* mAmmo;
	Text* mProfileDump[15];
	bool mShowingProfile;
public:
	
	/**
	 * Default constructor.
	 */
	HUD();
	
	/**
	 * Deconstructor.
	 */
	~HUD();
	
	void Draw();
};