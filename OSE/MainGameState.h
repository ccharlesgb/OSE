#pragma once

#include "igamestate.h"
#include "Box2d.h"

class MainGameState : public IGameState , public b2ContactListener
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

	void BeginContact(b2Contact* contact)
	{
		contact->GetFixtureA();
	}

 

  void EndContact(b2Contact* contact)

  { /* handle end event */ }

 

  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
  { /* handle pre-solve event */ }

 

  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)

  { /* handle post-solve event */ }

};

