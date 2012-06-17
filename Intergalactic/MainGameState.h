#pragma once

#include "igamestate.h"
#include "Box2d.h"

class MainGameState : public IGameState
{
private:
	BaseObject* Player;
	BaseObject* Map;
	b2World *mWorld;
	double mLastPhysics;
	double mPhysAcc;
	std::vector<BaseObject*> mPlanets;
	BaseObject* bg;
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

	void CreatePlanetarySystem(Vector2 Origin);
};

