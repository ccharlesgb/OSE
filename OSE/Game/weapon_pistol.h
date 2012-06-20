#pragma once

#include "../Engine/BaseObject.h"

class weapon_pistol : public BaseObject
{
public:
	weapon_pistol(void);
	~weapon_pistol(void);

	static void PrimaryFire(BaseObject* ent, VariantMap &Data);
};

