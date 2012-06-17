#pragma once
#include "BaseRenderable.h"
#include <iostream>

class StarField : public BaseRenderable
{
private:

public:
	StarField(void);
	~StarField(void);
	void Think();
};

