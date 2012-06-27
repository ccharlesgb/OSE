#pragma once

#include <stdlib.h>
#include <vector>
#include "SFML/Graphics.hpp"
#include "../Utilities/Vector2.h"
#include "../Render/DebugDraw.h"
#include "../GameGlobals.h"
#include "../EntityList.h"
//#include <LTBL/Light/LightSystem.h>
#include "BaseHUD.h"

class Camera;
class IGameState;
class BaseObject;

#define RENDERER Renderer::Instance()
class Renderer : public IEntityListener<BaseObject*>
{
private:
	sf::RenderWindow *mRender;
	//ltbl::LightSystem *mLightSystem;
	sf::View mView;
	DebugDraw* mPhysDebug;
	
	BaseHUD *HUD;
	
	EntityList<BaseObject*> Renderables;
	EntityList<BaseObject*> OnScreenEnts;
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
	void AddEntity(BaseObject* render);
	void Clear() {mRender->clear();};
	void Display() {mRender->display();};
	void Draw(IGameState *State);
	void SetWindow(sf::RenderWindow *wind);
	
	void SetHUD(BaseHUD *hud) { HUD = hud; };

	void UpdateOnScreenList();
};
