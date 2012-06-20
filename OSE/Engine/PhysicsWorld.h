#pragma once

#include <Box2D.h>
#include "../Engine/PhysicsDef.h"
#include "../Engine/BaseObject.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Vector2.h"

#define VelIterations 8
#define PosIterations 3

class QueryCallback : public b2QueryCallback
{
public:
    QueryCallback(const b2Vec2& point)
    {
        m_point = point;
        m_object = NULL;
    }

    bool ReportFixture(b2Fixture* fixture)
    {
        if (fixture->IsSensor()) return true; //ignore sensors
        bool inside = fixture->TestPoint(m_point);
        if (inside)
        {
             // We are done, terminate the query.
             m_object = fixture->GetBody();
                 return false;
        }

        // Continue the query.
        return true;
    }

    b2Vec2  m_point;
    b2Body* m_object;
};

class PhysicsWorld : public b2ContactListener
{
private:
	b2World *mWorld;
public:
	PhysicsWorld(void);
	~PhysicsWorld(void);

	void Step(float delta)
	{
		mWorld->Step(delta, VelIterations, PosIterations);
	};

	void AddPhysicsDef(PhysicsDef* def);
	void DrawDebug()
	{	
		mWorld->SetDebugDraw(gGlobals.PhysicsDebugDraw);
		mWorld->DrawDebugData();
	};

	BaseObject* QueryPoint(Vector2 Point);

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
	}

	void EndContact(b2Contact* contact)
	{
		/* handle end event */ 
	}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{ 
		/* handle pre-solve event */ 
	}

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{ 
		/* handle post-solve event */
	}
};

