#pragma once

#include <SFML/Graphics.hpp>
#include "../GameGlobals.h"
#include "../Utilities/Vector2.h"

class LightInfo
{
private:
	Vector2 mPos;
	sf::Sprite mLightSprite;
	sf::RenderTexture rendertex;
	sf::Color mColour;
	float mRadius;
public:
	LightInfo(void);
	~LightInfo(void);

	void UpdateLightSprite();

	void SetRadius(float radius) {mRadius = radius;};
	float GetRadius() {return mRadius;};

	void SetPosition(const Vector2 &pos) {mPos = pos;};
	Vector2 GetPosition() {return mPos;};

	sf::Sprite* GetSprite() {return &mLightSprite;};

	void SetColour(Colour col) {mColour = col.SF();};
	sf::Color GetColour() {return mColour;};
};

