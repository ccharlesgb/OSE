#include "Sun.h"
#include "IGameState.h"

#define GRAVITY_STRENGTH 150

LINKCLASSTONAME("ent_sun", Sun)

Sun::Sun(void)
{
	SetRenderType(RENDER_SPRITE);
	InitRenderer();
	GetRenderer()->SetDrawOrder(RENDERGROUP_ENTITIES);
	PhysicsInit(DYNAMIC_BODY);
	mRadius = ig::Random(700,1500);
}

void Sun::Spawn()
{
	GetRenderer()->SetTexture("sun.png", true);
	GetRenderer()->SetSize(Vector2(mRadius * 2.f,mRadius * 2.f));
	SetDrawPriority(0);
	CircleShape* shape = new CircleShape;
	shape->mRadius = (mRadius * 0.86);
	GetPhysObj()->AddPhysicsShape(shape);
	GetPhysObj()->SetAngularDamping(0);
	GetPhysObj()->SetLinearDamping(0.5);

	int Planet_Count = ig::Random(1, 5);
	float system_radius = mRadius + 200;
	for (int i=0; i<Planet_Count; i++)
	{
		float size = ig::Random(100,500);
		float distance = (i * i * ig::Random(size, size * 3));
		system_radius += (distance + size * 2);
		CreatePlanet(size, system_radius + distance);
	}
}

void Sun::CreatePlanet(float size, float distance)
{
	BasePhysics* plan = dynamic_cast<BasePhysics*>(CreateEntity("ent_test"));
	mPlanets.push_back(plan);

	float fudge = 500;
	VariantMap dat;
	dat.AddData("radius", (int)size);
	plan->Fire("SetRadius", dat);
	dat.Clear();
	dat.AddData("sun", this);
	float theta = ig::Random(0,2*3.41f);
	plan->SetPos((Vector2(std::cos(theta), std::sin(theta)) * distance) + GetPos());
	plan->Fire("SetSun", dat);
	plan->Spawn();
	plan->SetAngularVelocity(ig::Random(30,60));
	///if (ig::RandomInt(0,1) == 1)
	//	plan->SetAngularVelocity(GetAngularVelocity() * -1);
	//std::cout << "ANG VEL: " << GetAngularVelocity() << "\n";
	dat.Clear();
}

void Sun::Think()
{

}

Sun::~Sun(void)
{
}
