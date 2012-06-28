#pragma once

#include "../Engine/Bases/BaseObject.h"
#include "../Engine/Render/Line.h"

class weapon_pistol : public BaseObject
{
private:
	float mNextPrimaryFire;
	float mLastShot;
	Line* mLine;

	int mPrimAmmo;
	int mSecoAmmo;

	int mPrimClipSize;
	int mPrimClip;
	int mSecoClipSize;
	int mSecoClip;
	
	bool mReloading;
public:
	weapon_pistol(void);
	~weapon_pistol(void);

	static void PrimaryFire(BaseObject* ent, VariantMap &Data);
	void SetNextPrimaryFire(float time) {mNextPrimaryFire = time;};
	float GetNextPrimaryFire() {return mNextPrimaryFire;};

	void ShootBullet();

	void Spawn();
	void Draw();

	void SetPrimaryClipSize(int size) {mPrimClipSize = size;};
	int GetPrimaryClipSize() {return mPrimClipSize;};

	void SetSecondaryClipSize(int size) {mSecoClipSize = size;};
	int GetSecondaryClipSize() {return mSecoClipSize;};

	void UsePrimaryAmmo(int amount);
	void UseSecondaryAmmo(int amount);

	void SetPrimaryAmmo(int ammo) {mPrimAmmo = ammo;};
	int GetPrimaryAmmo() {return mPrimAmmo;};

	void SetSecondaryAmmo(int ammo) {mSecoAmmo = ammo;};
	int GetSecondaryAmmo() {return mSecoAmmo;};

	void RestockPrimaryClip() {mPrimAmmo -= (mPrimClipSize - mPrimClip); mPrimClip = mPrimClipSize;};
	void RestockSecondaryClip() {mSecoClip = mSecoClipSize; mSecoAmmo -= mSecoClipSize;};


	void Reload();
	bool IsReloading() {return mReloading;};

	void Think();
};

