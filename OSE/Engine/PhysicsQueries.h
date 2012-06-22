#pragma once

#include <Box2D.h>
#include "../Engine/Utilities/Vector2.h"
#include "../Engine/BaseObject.h"

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

class TraceResult
{
public:
	TraceResult()
	{
		mHitEnt = NULL;
		mHitNormal = Vector2();
		mHitPos = Vector2();
	}
	BaseObject* mHitEnt;
	Vector2 mHitPos;
	Vector2 mHitNormal;
};

class TraceInfo
{
public:
	Vector2 mStartPoint;
	Vector2 mEndPoint;
};

//Callback class to return the first body in a AABB Query
class TraceQueryCallback : public b2RayCastCallback
{
public:
    TraceQueryCallback(const TraceInfo info, TraceResult *result)
    {
		mInfo = info;
		mBody = NULL;
		mResult = result;
    }

	float32 ReportFixture(b2Fixture* fixture,
			const b2Vec2& point,const b2Vec2& normal, float32 fraction)
    {
        if (fixture->IsSensor()) return -1; //ignore sensors
        // We are done, terminate the query.
        mBody = fixture->GetBody();
		if (mBody)
		{
			mResult->mHitEnt = static_cast<BaseObject*>(mBody->GetUserData());
		}
		mResult->mHitNormal = normal;
		mResult->mHitPos = point;

        return fraction;
    }

	b2Body *mBody;
    TraceInfo mInfo;
	TraceResult *mResult;
};

class PhysicsQueries
{
private:
	static b2World* mWorld;
public:
	static void SetWorld(b2World* world) {if (mWorld == NULL) mWorld = world;};
	static BaseObject* QueryPoint(Vector2 Point);
	static void TraceLine(TraceInfo& info, TraceResult *result);
};

