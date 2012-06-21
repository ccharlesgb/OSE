#include "PhysicsQueries.h"
#include "../Engine/PhysicsDef.h"

b2World* PhysicsQueries::mWorld = NULL;

BaseObject* PhysicsQueries::TraceLine(TraceInfo& info)
{
	std::cout << " TRACING\n";
	TraceQueryCallback* callback;
	std::cout << " TRACING\n";
	mWorld->RayCast(callback, info.mStartPoint.B2(), info.mEndPoint.B2());
	std::cout << " TRACING\n";
	BaseObject* ent = NULL;
	if (callback->mBody != NULL)
		ent = static_cast<BaseObject*>(callback->mBody->GetUserData());
	std::cout << " TRACING\n";
	return ent;
}

BaseObject* PhysicsQueries::QueryPoint(Vector2 Point)
{
	PointQueryCallback result((Point / PIXELS_PER_METRE).B2());
	b2AABB box;
	float size = 0.01f;
	box.lowerBound = (Point / PIXELS_PER_METRE - Vector2(size,size)).B2();
	box.upperBound = (Point / PIXELS_PER_METRE + Vector2(size,size)).B2();
	mWorld->QueryAABB(&result, box);
	b2Body *body = result.m_object;
	//std::cout << "Object: " << body << "\n";
	if (body)
	{
		return static_cast<BaseObject*>(body->GetUserData());
	}
	return NULL;
}
