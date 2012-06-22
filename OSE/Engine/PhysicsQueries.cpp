#include "PhysicsQueries.h"
#include "../Engine/PhysicsDef.h"

b2World* PhysicsQueries::mWorld = NULL;

void PhysicsQueries::TraceLine(TraceInfo& info, TraceResult &result)
{
	TraceQueryCallback callback(info, &result);
	mWorld->RayCast(&callback, (info.mStartPoint / PIXELS_PER_METRE).B2(), (info.mEndPoint / PIXELS_PER_METRE).B2());
	std::cout << callback.mResult->mHitEnt << "\n";
	result = *callback.mResult;
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
	if (body)
	{
		return static_cast<BaseObject*>(body->GetUserData());
	}
	return NULL;
}
