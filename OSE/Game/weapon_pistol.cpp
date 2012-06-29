#include "weapon_pistol.h"
#include "../Engine/PhysicsQueries.h"
#include "../Engine/GameGlobals.h"

LINKCLASSTONAME("weapon_pistol", weapon_pistol)

weapon_pistol::weapon_pistol(void)
{
	RegisterInput("fire1", PrimaryFire);
	CreateSound("shot", "gunshot");
	CreateSound("reload", "reload");
	RenderInit();
	GetSound("shot")->SetVolume(20.f);
	SetDrawOrder(RENDERGROUP_ENTITIES);
	mLine = new Line(gGlobals.RenderWindow);
	mLine->SetColour(Colour(255, 236, 139));
	mNextPrimaryFire = 0.f;
	mLastShot = 0.f;
	mPrimClip = 0;
	mSecoClip = 0;
	SetPrimaryAmmo(5000);
	SetPrimaryClipSize(40);
	RestockPrimaryClip();
	RestockSecondaryClip();
}

weapon_pistol::~weapon_pistol(void)
{
}

void weapon_pistol::Spawn()
{
	SetModel("muzzle_flash", 0.2f);
}

void weapon_pistol::Reload()
{
	if (IsReloading())
		return;
	SetNextPrimaryFire(gGlobals.CurTime + 3.5f);
	GetSound("reload")->Play();
	RestockPrimaryClip();
	mReloading = true;
}

void weapon_pistol::Draw()
{
	if (mLastShot + 0.05 > gGlobals.CurTime)
	{
		mLine->Draw();
		DrawModel();
	}
}

void weapon_pistol::Think()
{
}

void weapon_pistol::UsePrimaryAmmo(int amount)
{
	mPrimClip = ig::Approach(mPrimClip, 0, amount);
	if (mPrimClip <= 0)
		Reload();
}

void weapon_pistol::UseSecondaryAmmo(int amount)
{
	mSecoClip = ig::Approach(mPrimClip, 0, amount);
	if (mSecoClip <= 0)
		Reload();
}

void weapon_pistol::ShootBullet()
{
	TraceInfo info;
	info.mStartPoint = GetPos() + (GetForward() * -10.f);
	float range = 2000;
	float spread = 0.07f; //In radians
	info.mEndPoint = GetPos() + (GetForward() * range) + (GetRight() * ig::Random(-spread,spread) * range);
	TraceResult TraceRes;
	PhysicsQueries::TraceLine(info, &TraceRes);

	//Tracer Effect
	mLine->mVerts[0] = info.mStartPoint;
	mLine->mVerts[1] = TraceRes.mHitPos + (GetForward() * 10.f); // Add some slop to prevent line stopping before sprite

	if (TraceRes.mHitEnt != NULL)
	{
		DamageInfo info;
		info.Amount = ig::Random(15,25);
		info.type = DAMAGETYPE_BULLET;
		info.Inflictor = GetOwner();
		TraceRes.mHitEnt->TakeDamage(info);
	}
}

void weapon_pistol::PrimaryFire(BaseObject* ent, VariantMap &Data)
{
	weapon_pistol* me = dynamic_cast<weapon_pistol*>(ent);
	if (me->GetNextPrimaryFire() < gGlobals.CurTime)
	{	
		me->mReloading = false;
		if (me->GetPrimaryAmmo() <= 0)
			return;
		me->EmitSound("shot");
		me->ShootBullet();
		me->mLastShot = (float)gGlobals.CurTime;
		me->SetNextPrimaryFire((float)gGlobals.CurTime + 0.1f);
		me->UsePrimaryAmmo(1);
	}
}
