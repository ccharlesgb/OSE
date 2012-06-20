#pragma once

#include <vector>
#include "Utilities/Vector2.h"
#include <Box2D.h>

#define PIXELS_PER_METRE 64.f
#define METRES_PER_PIXEL 1/PIXELS_PER_METRE

class BaseObject;


class CollisionInfo
{
public:
	BaseObject* OtherEnt;
};

//What kind of Box2D body are we?
enum BODY_TYPE
{
	STATIC_BODY = b2_staticBody,
	DYNAMIC_BODY = b2_dynamicBody,
	KINEMATIC_BODY = b2_kinematicBody
};

//BaseShape: Used to define physics shapes for box 2d.
class BaseShape
{
public:
	float mDensity;
	BaseShape() {mDensity = 0.1f;};
};

//CircleShape: Circle Primitive
class CircleShape : public BaseShape
{
public:
	float mRadius;
};

//PolygonShape: ConVEX polygon shape.
class PolygonShape : public BaseShape
{
private:
	int mVertexCount;
public:
	PolygonShape()
	{
		mVertexCount = 0;
	}
	~PolygonShape()
	{
		
	}
	Vector2 mVertices[16]; //TODO: MAKE THIS DYNAMIC?
	int GetVertexCount() {return mVertexCount;};
	void AddVertex(const Vector2 &v) {mVertices[mVertexCount] = v; mVertexCount++;};
};

/*PhysicsDef: 
	The Class that controls everything to do with physics on an entity,
	kinda like a PhysicsObject.
*/
			 
class PhysicsDef
{
private:
	b2Body *mPhysObj;
	b2World *mWorld;

	Vector2 mPosition;
	float mAngle;

	BODY_TYPE mType;
	BaseObject* mOwner;
public:
	PhysicsDef(void);
	~PhysicsDef(void);

	void SetPos(float x, float y) 
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

	void SetOwner(BaseObject* own) {mOwner = own;};
	BaseObject* GetOwner() {return mOwner;};

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

