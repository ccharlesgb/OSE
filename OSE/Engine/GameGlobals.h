#pragma once

#include "Render/DebugDraw.h"
#include "EntityListGlobal.h"

class BaseObject;
class sf::RenderWindow;

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
	sf::RenderWindow* RenderWindow;
	bool EnableRenderCulling;
};

extern GameGlobals gGlobals;