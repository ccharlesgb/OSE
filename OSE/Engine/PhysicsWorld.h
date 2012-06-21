#pragma once

#include <Box2D.h>
#include "../Engine/PhysicsDef.h"
#include "../Engine/BaseObject.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Utilities/Vector2.h"

#define VelIterations 5
#define PosIterations 3

//Wrapper Class for b2World. Put ALL physics related stuff here
class PhysicsWorld : public b2ContactListener
{
private:
	b2World *mWorld;
	std::vector<b2JointDef*> mJointQueue;
public:
	PhysicsWorld(void);
	~PhysicsWorld(void);

	void AddJoint(b2JointDef* joint);

	void Step(float delta);

	void AddPhysicsDef(PhysicsDef* def);
	void DrawDebug()
	{
		mWorld->SetDebugDraw(gGlobals.PhysicsDebugDraw);
		mWorld->DrawDebugData();
	};

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

