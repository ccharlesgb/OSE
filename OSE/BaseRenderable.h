#pragma once

#include "Vector2.h"
#include "MathUtils.h"
#include "SFML/Graphics.hpp"
#include "VariantMap.h"
#include "BaseObject.h"

class BaseRenderable : public BaseObject
{
private:
	BaseRenderable(const BaseRenderable&);
	BaseRenderable& operator=(const BaseRenderable&);

public:
	BaseRenderable(void);
	virtual ~BaseRenderable(void);

	//Rendering
	void InitRenderer();

	Vector2 GetScreenPos() {};

	void SetDrawPriority(int prior) {};
	int GetDrawPriotity() {};

	//Interface to renderer
	void SetColour(sf::Color col) {};
	sf::Color GetColour() {};

	void Tick();
};
