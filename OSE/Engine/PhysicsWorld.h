#pragma once

#include <Box2D.h>
#include "../Engine/PhysicsDef.h"
#include "../Engine/BaseObject.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Utilities/Vector2.h"

#define VelIterations 5
#define PosIterations 3

//Callback class to return the first body in a AABB Query
class PointQueryCallback : public b2QueryCallback
{
public:
    PointQueryCallback(const b2Vec2& point)
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

//Callback class to return the first body in a AABB Query
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

class TraceInfo
{
public:
	Vector2 mStartPoint;
	Vector2 mEndPoint;
};

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

	BaseObject* QueryPoint(Vector2 Point);
	BaseObject* TraceLine(TraceInfo& info);

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
		//b2WeldJointDef* joint = new b2WeldJointDef();
		//joint->Initialize(bodyA, bodyB, bodyA->GetPosition());
		//AddJoint(joint);
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

