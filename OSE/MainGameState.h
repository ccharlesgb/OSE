#pragma once

#include "igamestate.h"
#include "Box2d.h"
#include "PhysicsDef.h"

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
		b2Body *bodyA = contact->GetFixtureA()->GetBody();
		b2Body *bodyB = contact->GetFixtureB()->GetBody();
		BaseObject* EntA = static_cast<BaseObject*>(bodyA->GetUserData()); //GetUserData() returns a pointer to the owner!
		BaseObject* EntB = static_cast<BaseObject*>(bodyB->GetUserData());
		CollisionInfo info;
		info.OtherEnt = EntB;
		EntA->StartTouch(&info);
		info.OtherEnt = EntA;
		EntB->StartTouch(&info);
		std::cout << "Collsion between: " << EntA->GetClassName() << " and : " << EntB->GetClassName() << "\n";
	}

 

  void EndContact(b2Contact* contact)

  { /* handle end event */ }

 

  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
  { /* handle pre-solve event */ }

 

  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)

  { /* handle post-solve event */ }

};

