#pragma once
#include "BaseObject.h"
#include "PhysicsDef.h"

class BasePhysics : public BaseObject
{
private:

public:
	BasePhysics(void);
	~BasePhysics(void);

	//Physics

	void PhysicsInit(BODY_TYPE typ);

	void Tick();

	//Inline
	void SetPos(Vector2 p) {if (mPhysObj) mPhysObj->SetPos(p); BaseObject::SetPos(p);};
	void SetAngle(float a) {if (mPhysObj) mPhysObj->SetAngle(a); BaseObject::SetAngle(a);};

	void SetVelocity(Vector2 vel)
	{
		mPhysObj->SetLinearVelocity(vel);
	};
	Vector2 GetVelocity()
	{
		return Vector2(mPhysObj->GetLinearVelocity());
	};
	void SetAngularVelocity(float angvel)
	{
		mPhysObj->SetAngularVelocity(angvel);
	};
	float GetAngularVelocity()
	{
		return mPhysObj->GetAngularVelocity();
	};
	void ApplyLinearImpulse(Vector2 imp, Vector2 offset)
	{
		mPhysObj->ApplyLinearImpulse(imp, offset);
	};
	void ApplyForceCenter(Vector2 force)
	{
		mPhysObj->ApplyForce(force, GetPhysObj()->GetPos());
	}
	void ApplyForce(Vector2 force, Vector2 Offset)
	{
		mPhysObj->ApplyForce(force, Offset);
	}
	void ApplyTorque(float torque)
	{
		GetPhysObj()->ApplyTorque(torque);
	}
};

