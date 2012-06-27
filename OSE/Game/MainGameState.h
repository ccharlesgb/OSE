#pragma once

#include "../Engine/IGameState.h"
#include "../Engine/PhysicsWorld.h"
#include "HUD.h"

class MainGameState : public IGameState
{
private:
	BaseObject* Player;
	BaseObject* Map;
	PhysicsWorld mPhysicsWorld;
	double mLastPhysics;
	double mPhysAcc;
	std::vector<BaseObject*> mPlanets;
	BaseObject* bg;
	HUD *Hud;
public:
	MainGameState(void);
	~MainGameState(void);

	void Initialize();
	void ShutDown();
	void Tick();
	void OnEvent(sf::Event &Event);
	void OnKeyPressed(sf::Keyboard::Key Key, bool Pressed);
	void OnMouseButtonPressed(sf::Mouse::Button Button, bool Pressed);
	void OnEntityCreated(BaseObject* ent);
	void DrawDebugData();

	void AdjustZoom(float zoom_target);

	void CreatePlanetarySystem(Vector2 Origin);

};

