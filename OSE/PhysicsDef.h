#pragma once

#include <vector>
#include "Vector2.h"
#include <Box2D.h>

#define PIXELS_PER_METRE 64.f
#define METRES_PER_PIXEL 1/PIXELS_PER_METRE

enum BODY_TYPE
{
	STATIC_BODY = b2_staticBody,
	DYNAMIC_BODY = b2_dynamicBody
};

class BaseShape
{
public:
	float mDensity;
	BaseShape() {mDensity = 0.1f;};
};

class CircleShape : public BaseShape
{
public:
	float mRadius;
};

class PolygonShape : public BaseShape
{
public:
	std::vector<Vector2> mVertices;
	int GetVertexCount() {return mVertices.size();};
	void AddVertex(Vector2 v) {mVertices.push_back(v);};
};

class PhysicsDef
{
private:
	b2Body *mPhysObj;
	b2World *mWorld;

	Vector2 mPosition;
	float mAngle;

	BODY_TYPE mType;
public:
	PhysicsDef(void);
	~PhysicsDef(void);

	void PhysicsDef::SetPos(float x, float y) 
	{
		SetPos(Vector2(x,y));
	};

	std::vector<Vector2> mVertices;

	void SetType(BODY_TYPE typ) {mType = typ;};
	BODY_TYPE GetType() {return mType;};

	void CreatePhysics(b2World *World);

	void SetPos(Vector2 p);
	Vector2 GetPos();
	
	void SetAngle(float a);
	float GetAngle();

	float GetMass() {return mPhysObj->GetMass() * PIXELS_PER_METRE;};

	void AddPhysicsShape(PolygonShape* hull);
	void AddPhysicsShape(CircleShape* hull);

	void SetLinearVelocity(Vector2 vel)
	{
		mPhysObj->SetLinearVelocity((vel * METRES_PER_PIXEL).B2());
	};
	Vector2 GetLinearVelocity()
	{
		return Vector2(mPhysObj->GetLinearVelocity()) * PIXELS_PER_METRE;
	};
	void SetAngularVelocity(float angvel)
	{
		mPhysObj->SetAngularVelocity(-ig::DegToRad(angvel));
	};
	float GetAngularVelocity()
	{
		return ig::RadToDeg(-mPhysObj->GetAngularVelocity());
	};
	void ApplyLinearImpulse(Vector2 imp, Vector2 offset)
	{
		mPhysObj->ApplyLinearImpulse((imp * METRES_PER_PIXEL).B2(), (offset * METRES_PER_PIXEL).B2());
	};
	void ApplyForce(Vector2 force, Vector2 offset)
	{
		mPhysObj->ApplyForce((force * METRES_PER_PIXEL).B2(), (offset * METRES_PER_PIXEL).B2());
	};
	void ApplyTorque(float torque) {mPhysObj->ApplyTorque(torque * METRES_PER_PIXEL);};
	void SetAngularDamping(float d) {mPhysObj->SetAngularDamping(d);};
	void SetLinearDamping(float d) {mPhysObj->SetLinearDamping(d);};
};

