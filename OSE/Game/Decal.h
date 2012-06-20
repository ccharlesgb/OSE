#pragma once
#include "../Engine/BaseObject.h"

class Decal : public BaseObject
{
private:
	
public:
	Decal(void);
	~Decal(void);
	void Spawn();
	void Think();
};