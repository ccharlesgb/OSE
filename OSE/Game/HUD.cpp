#include "HUD.h"
#include "../Engine/Render/Renderer.h"
#include "../Engine/EntityListGlobal.h"
#include "../Engine/InputHandler.h"
#include "../Engine/Profiler.h"
#include "weapon_pistol.h"
#include "Player.h"

HUD::HUD()
{
	RENDERER->SetHUD(this);
	mHealth = new Text(gGlobals.RenderWindow);
	mHealth->SetText("Health: ");
	mHealth->SetUseScreenCoords(true);
	mHealth->SetPosition(Vector2(10.f, gGlobals.GameHeight * 0.95f));

	mAmmo = new Text(gGlobals.RenderWindow);
	mAmmo->SetText("Ammo: ");
	mAmmo->SetUseScreenCoords(true);
	mAmmo->SetPosition(Vector2(gGlobals.GameWidth * 0.85f, gGlobals.GameHeight * 0.95f));

	for (int i=0; i<PROFILE_TYPE_COUNT; i++)
	{
		mProfileDump[i] = new Text(gGlobals.RenderWindow);
		mProfileDump[i]->SetUseScreenCoords(true);
		mProfileDump[i]->SetPosition(Vector2(10, i * 25));
		mProfileDump[i]->SetScale(0.6f);
	}
	mShowingProfile = false;
}

void HUD::Draw()
{
	ObjList* list = gGlobals.gEntList.FindByClass("player");
	if (list->GetSize() > 0)
	{
		Player* ply = dynamic_cast<Player*>(*list->FirstEnt()); // Get the (first) player
		char buffer[30];
#ifdef __APPLE__ 
			sprintf(buffer, "Health: %i", (int)ply->GetHealth());
#else
			sprintf_s(buffer, "Health: %i", (int)ply->GetHealth());
#endif
		mHealth->SetText(buffer);
		mHealth->Draw();

		weapon_pistol* wep = dynamic_cast<weapon_pistol*>(ply->GetActiveWeapon());
#ifdef __APPLE__ 
		sprintf(buffer, "Ammo: %i", wep->GetPrimaryAmmo());
#else
		sprintf_s(buffer, "Ammo: %i", wep->GetPrimaryAmmo());
#endif
		mAmmo->SetText(buffer);
		mAmmo->Draw();
	}

	if (InputHandler::IsKeyPressed(sf::Keyboard::F4))
	{
		mShowingProfile = !mShowingProfile;
	}
	if (mShowingProfile)
	{
		ProfileInformation* mExecTimes = Profiler::GetLastFrameInfo();
		for(int i=0; i<PROFILE_TYPE_COUNT; i++)
		{
			if (mExecTimes->NAME[i].empty())
				continue;
			char buffer[60];
			std::string INDENT;
			for(int spaces = 0; spaces < mExecTimes->DEPTH[i] * 4; spaces++)
			{
				INDENT = INDENT + ' ';
			}
			float perc = mExecTimes->TIME[i] / mExecTimes->TIME[0];
			mProfileDump[i]->SetColour(Colour(255, 255 - (255 * perc),255 - (255 * perc)));

			if (i == 0) //WHOLE TICK
			{
				float fps = (1000.f/mExecTimes->TIME[i]);
				if (fps > 60.f)
					mProfileDump[i]->SetColour(Colour(0,255,0));
#ifdef __APPLE__
				sprintf(buffer, (INDENT + mExecTimes->NAME[i] + ": %f (FPS: %f)").c_str(),mExecTimes->TIME[i], fps);
#else
				sprintf_s(buffer, (INDENT + mExecTimes->NAME[i] + ": %f (FPS: %f)").c_str(),mExecTimes->TIME[i], fps);
#endif
			}
			else
			{
#ifdef __APPLE__
				sprintf(buffer, (INDENT + mExecTimes->NAME[i] + ": %f").c_str(), mExecTimes->TIME[i]);
#else
				sprintf_s(buffer, (INDENT + mExecTimes->NAME[i] + ": %f").c_str(), mExecTimes->TIME[i]);
#endif
			}
			mProfileDump[i]->SetText(buffer);
			mProfileDump[i]->Draw();
		}
	}
}

