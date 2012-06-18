#pragma once

#include <stdlib.h>
#include <vector>
#include "SFML/Graphics.hpp"
#include "BaseRender.h"
#include "Vector2.h"
#include <vector>
#include "DebugDraw.h"
#include "GameGlobals.h"
#include "EntityList.h"

class Camera;
class IGameState;

#define RENDERER Renderer::Instance()
class Renderer : public IEntityListener<BaseObject*>
{
private:
	sf::RenderWindow *mRender;
	sf::View mView;
	DebugDraw* mPhysDebug;
	EntityList<BaseRender*> Renderers;
public:
	Vector2 GameToSFML(Vector2 Pos);
	Vector2 ToScreen(Vector2 pos);
	Renderer(void);
	~Renderer(void);
	static Renderer* Instance();
	static void Cleanup();
	void OnResize();
	void OnEntityAdded(BaseObject* ent);
	void OnEntityRemoved(BaseObject* ent);
	void AddRenderer(BaseRender* render);
	bool IsVisible(BaseRender* rend);
	void Clear() {mRender->clear();};
	void Display() {mRender->display();};
	void Draw(IGameState *State);
	void SetWindow(sf::RenderWindow *wind);
};
