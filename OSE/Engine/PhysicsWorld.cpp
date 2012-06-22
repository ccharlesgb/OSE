
#include "PhysicsWorld.h"
#include "InputHandler.h"
#include "PhysicsQueries.h"

PhysicsWorld::PhysicsWorld(void)
{
	mWorld = new b2World(b2Vec2(0.f,0.f));
	mWorld->SetContactListener(this);
	PhysicsQueries::SetWorld(mWorld);
}

PhysicsWorld::~PhysicsWorld(void)
{
	delete mWorld;
}

void PhysicsWorld::AddJoint(b2JointDef* joint)
{
	mJointQueue.push_back(joint);
}

void PhysicsWorld::Step(float delta)
{
	//Go through any constraints that need to be added and add them to the world
	std::vector<b2JointDef*>::iterator iter;
	iter = mJointQueue.begin();
	while (iter != mJointQueue.end())
	{
		mWorld->CreateJoint(*iter);
		delete *iter;
		iter++;
	}
	mJointQueue.clear(); //Clear the queue for next time
	mWorld->Step(delta, VelIterations, PosIterations);
}

void PhysicsWorld::AddPhysicsDef(PhysicsDef* def)
{
	def->CreatePhysics(mWorld);
}

void PhysicsWorld::BeginContact(b2Contact* contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();
	BaseObject* EntA = static_cast<BaseObject*>(bodyA->GetUserData()); //GetUserData() returns a pointer to the owner!
	BaseObject* EntB = static_cast<BaseObject*>(bodyB->GetUserData());
	CollisionInfo info;

	info.Speed = (EntA->GetPhysObj()->GetLinearVelocity() - EntB->GetPhysObj()->GetLinearVelocity()).Length();
	info.OtherEnt = EntB;
	EntA->StartTouch(&info);
	info.OtherEnt = EntA;
	EntB->StartTouch(&info);
	//b2WeldJointDef* joint = new b2WeldJointDef();
	//joint->Initialize(bodyA, bodyB, bodyA->GetPosition());
	//AddJoint(joint);
}

void PhysicsWorld::EndContact(b2Contact* contact)
{
}

void PhysicsWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void PhysicsWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{ 
}