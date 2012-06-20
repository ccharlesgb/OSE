#include "PhysicsWorld.h"


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
