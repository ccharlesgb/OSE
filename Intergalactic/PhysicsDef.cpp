#include "PhysicsDef.h"

#define MAX_VERTEX 32

PhysicsDef::PhysicsDef(void)
{
	mPhysObj = NULL;
	mPosition = Vector2(0,0);
	mAngle = 0;
}

PhysicsDef::~PhysicsDef(void)
{
	mWorld->DestroyBody(mPhysObj);
}

void PhysicsDef::SetPos(Vector2 p) 
{
	if (mPhysObj)
		mPhysObj->SetTransform((p * METRES_PER_PIXEL).B2(), ig::DegToRad(GetAngle()));
	mPosition = p * METRES_PER_PIXEL;
}

Vector2 PhysicsDef::GetPos()
{
	if (mPhysObj)
	{
		mPosition = mPhysObj->GetPosition();
		//mPosition = mPosition * PIXELS_PER_METRE;
	}
	return mPosition * PIXELS_PER_METRE;
}
	
void PhysicsDef::SetAngle(float a) 
{
	mAngle = a;
	if (mPhysObj)
	{ 
		mPhysObj->SetTransform((GetPos() * METRES_PER_PIXEL).B2(), ig::DegToRad(a));
	}
}

float PhysicsDef::GetAngle()
{
	if (mPhysObj)
	{
		mAngle = -ig::RadToDeg(mPhysObj->GetAngle());
	}
	return mAngle;
}

void PhysicsDef::CreatePhysics(b2World *World)
{
	mWorld = World;
	b2BodyDef def;
	def.position = mPosition.B2();
	switch (GetType())
	{
	case STATIC_BODY:
		def.type = b2_staticBody;
		break;
	case DYNAMIC_BODY:
		def.type = b2_dynamicBody;
		break;
	default:
		std::cout << "INVALID BODY TYPE\n";
	}
	def.angularDamping = 0;
	def.linearDamping = 0;
	mPhysObj = World->CreateBody(&def);

}

void PhysicsDef::AddPhysicsShape(PolygonShape* hull) 
{
	b2Vec2 verts[MAX_VERTEX];
	for (int i=0; i <= hull->GetVertexCount(); i++)
	{
		hull->mVertices[i].x = hull->mVertices[i].x * METRES_PER_PIXEL;
		hull->mVertices[i].y = hull->mVertices[i].y * METRES_PER_PIXEL;
		verts[i] = hull->mVertices[i].B2();
	}
	b2PolygonShape fix;
	fix.Set(verts, hull->GetVertexCount());
	mPhysObj->CreateFixture(&fix, hull->mDensity);
};

void PhysicsDef::AddPhysicsShape(CircleShape* hull) 
{
	hull->mRadius = hull->mRadius * METRES_PER_PIXEL;
	b2CircleShape fix;
	fix.m_radius = hull->mRadius;
	mPhysObj->CreateFixture(&fix, hull->mDensity);
};
