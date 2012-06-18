#include "Test.h"
#include <iostream>

LINKCLASSTONAME("ent_test", Test)
#define GRAVITY_STRENGTH 150

Test::Test(void)
{
	RegisterInput("SetRadius", SetRadius);
	RegisterInput("SetImage", SetImage);
	RegisterInput("SetSun", SetSun);
	SetPos(Vector2::Random(-80.f,80.f));

	mRadius = ig::Random(10.f, 150.f);

	SetRenderType(RENDER_SPRITE);
	InitRenderer();
	GetRenderer()->SetDrawOrder(RENDERGROUP_ENTITIES);
	mImage = "planet.png";
	mSun = NULL;
	PhysicsInit(DYNAMIC_BODY);
}

void Test::SetRadius(BaseObject *ent, VariantMap &Data)
{
	Test* me = dynamic_cast<Test*>(ent);
	me->mRadius = Data.ReadInt("radius");
}

void Test::SetImage(BaseObject* ent, VariantMap &Data)
{
	Test* me = dynamic_cast<Test*>(ent);
	me->mImage = Data.ReadString("path");
}

void Test::Spawn()
{
	if (mRadius > 400)
		mImage = "planet3.png";
	GetRenderer()->SetTexture(mImage.c_str(), true);
	GetRenderer()->SetSize(Vector2(mRadius * 2.f,mRadius * 2.f));
	SetDrawPriority(0);
	CircleShape* shape = new CircleShape;
	shape->mRadius = (mRadius * 0.74);
	GetPhysObj()->AddPhysicsShape(shape);
	GetPhysObj()->SetAngularDamping(0);
	GetPhysObj()->SetLinearDamping(0);
}

void Test::SetSun(BaseObject* ent, VariantMap &Data)
{
	Test* me = dynamic_cast<Test*>(ent);
	me->mSun = dynamic_cast<BasePhysics*>(Data.ReadEntity("sun"));
	me->mDistance = (me->GetPos() - me->mSun->GetPos()).Length();
	float major_scale = ig::Random(1,1.5);
	me->mDistance = me->mDistance * major_scale;
	me->mSemiMajor = me->mDistance * major_scale;
	me->mSemiMinor = me->mDistance;
	float frac = me->mSemiMinor / me->mSemiMajor;
	me->mEcc = std::sqrt(1-(frac*frac));
	//std::cout << "ECC: " << me->mEcc << "\n";
	me->mThetaOffset = ig::Random(0, 6.82f);
	float avg_dist = me->mSemiMinor + me->mSemiMajor;
	avg_dist /= 2.f;
	float speed = std::sqrt((GRAVITY_STRENGTH * me->mSun->GetPhysObj()->GetMass() * 10000) / avg_dist);
	//std::cout << "speed: "  << speed << "\n";
	me->mVelocity = speed;
}

Test::~Test(void)
{
}

void Test::Think()
{
	if (mSun)
	{
		Vector2 pos_diff = GetPos() - mSun->GetPos();
		float dist = (pos_diff).Length();
		float theta = std::acosf(pos_diff.Normalize().Dot(Vector2(1,0))) + mThetaOffset;
		//std::cout << "PROGRESS: " << std::cos(theta) << "\n";
		float targ_dist = mSemiMajor* (1-mEcc * mEcc);
		targ_dist /= (1 + mEcc * cos(theta));
		//std::cout << "SEMI MAJOR: " << mSemiMajor << "SEMI MINOR: " << mSemiMinor << " TARG: " << targ_dist << "\n";
		float dist_diff = targ_dist - dist;
		float damping = -1 * (GetVelocity().Dot(pos_diff.Normalize()));
		ApplyForceCenter((pos_diff).Normalize() * (dist_diff + damping) * GetPhysObj()->GetMass() * 0.1f); //Apply radial correction
		
		Vector2 vel_dir = pos_diff.Normalize().Normal(); //The tangential direction
		float targ_vel = (mEcc* std::cos(theta) + 1) * mVelocity;
		float vel_diff = GetVelocity().Dot(vel_dir) - targ_vel; //Have we slowed down or sped up?

		ApplyForceCenter(vel_dir * -vel_diff * GetPhysObj()->GetMass() * 0.5);
	}
}
