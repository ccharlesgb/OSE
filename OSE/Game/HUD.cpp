#include "HUD.h"
#include "../Engine/Render/Renderer.h"

HUD::HUD()
{
	Renderer::Instance()->SetHUD(this);
	mHealth = new Text(gGlobals.RenderWindow);
	mHealth->SetText("HAX");
	mHealth->SetPosition(Vector2(0, 0));
}

void HUD::Draw()
{
	//mHealth->Draw();
}

