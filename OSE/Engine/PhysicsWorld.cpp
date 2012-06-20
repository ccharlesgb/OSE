#include "PhysicsWorld.h"
#include "InputHandler.h"


PhysicsWorld::PhysicsWorld(void)
{
	mWorld = new b2World(b2Vec2(0.f,0.f));
	mWorld->SetContactListener(this);
}

PhysicsWorld::~PhysicsWorld(void)
{
	delete mWorld;
}

void PhysicsWorld::AddPhysicsDef(PhysicsDef* def)
{
	def->CreatePhysics(mWorld);
}

BaseObject* PhysicsWorld::QueryPoint(Vector2 Point)
{
	QueryCallback result((InputHandler::GetMousePosWorld() / PIXELS_PER_METRE).B2());
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
