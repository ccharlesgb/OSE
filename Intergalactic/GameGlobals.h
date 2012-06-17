#pragma once

#include "DebugDraw.h"
#include "EntityListGlobal.h"

class BaseObject;

class GameGlobals
{
public:
	GameGlobals(void);
	~GameGlobals(void);
	double CurTime;
	double FrameTime;
	double RealTime;
	bool InFocus;
	int GameWidth;
	int GameHeight;
	BaseObject* Player;
	DebugDraw *PhysicsDebugDraw;
	EntityListGlobal gEntList;
};

extern GameGlobals gGlobals;