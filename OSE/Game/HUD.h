#pragma once

#include "../Engine/Render/BaseHUD.h"
#include "../Engine/Text.h"

class HUD : public BaseHUD
{
private:
	Text *mHealth;
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