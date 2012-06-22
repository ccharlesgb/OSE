#pragma once

#include "../Engine/BaseObject.h"
#include "../Engine/Line.h"

class weapon_pistol : public BaseObject
{
private:
	float mNextPrimaryFire;
	float mLastShot;
	Line* mLine;
public:
	weapon_pistol(void);
	~weapon_pistol(void);

	static void PrimaryFire(BaseObject* ent, VariantMap &Data);
	void SetNextPrimaryFire(float time) {mNextPrimaryFire = time;};
	float GetNextPrimaryFire() {return mNextPrimaryFire;};

	void Spawn();
	void Draw();
};

