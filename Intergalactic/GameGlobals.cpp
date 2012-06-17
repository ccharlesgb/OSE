#include "GameGlobals.h"

GameGlobals gGlobals;

GameGlobals::GameGlobals(void)
{
	CurTime = 0.f;
	FrameTime = 0.f;
	RealTime = 0.f;
	InFocus = false;
	GameWidth = -1;
	GameHeight = -1;
	Player = NULL;
	PhysicsDebugDraw = NULL;
}


GameGlobals::~GameGlobals(void)
{
}
