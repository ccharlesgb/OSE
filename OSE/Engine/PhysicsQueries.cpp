#include "PhysicsQueries.h"

BaseObject* PhysicsQueries::TraceLine(TraceInfo& info)
{
	TraceQueryCallback* callback;
	mWorld->RayCast(callback, info.mStartPoint.B2(), info.mEndPoint.B2());
	BaseObject* ent = static_cast<BaseObject*>(callback->mBody->GetUserData());
	return ent;
}

BaseObject* PhysicsQueries::QueryPoint(Vector2 Point)
{
	PointQueryCallback result((InputHandler::GetMousePosWorld() / PIXELS_PER_METRE).B2());
	b2AABB box;
	float size = 0.01f;
	box.lowerBound = (InputHandler::GetMousePosWorld() / PIXELS_PER_METRE - Vector2(size,size)).B2();
	box.upperBound = (InputHandler::GetMousePosWorld() / PIXELS_PER_METRE + Vector2(size,size)).B2();
	mWorld->QueryAABB(&result, box);
	b2Body *body = result.m_object;
	//std::cout << "Object: " << body << "\n";
	if (body)
	{
		return static_cast<BaseObject*>(body->GetUserData());
	}
	return NULL;
}
