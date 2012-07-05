#pragma once

#include <SFML/Graphics.hpp>
#include "../GameGlobals.h"
#include "../Utilities/Vector2.h"

class LightInfo
{
private:
	Vector2 mPos;
	float mAngle;
	sf::Sprite mLightSprite;
	sf::RenderTexture rendertex;
	sf::Color mColour;
	float mRadius;
	float mSpreadAngle; //How much of a circle are we?
public:
	LightInfo(void);
	~LightInfo(void);

	void UpdateLightSprite();

	void SetRadius(float radius) {mRadius = radius;};
	float GetRadius() {return mRadius;};

	void SetPosition(const Vector2 &pos) {mPos = pos;};
	Vector2 GetPosition() {return mPos;};

	sf::Sprite* GetSprite() {return &mLightSprite;};

	void SetColour(Colour col) {mColour = col.SF(); UpdateLightSprite();};
	sf::Color GetColour() {return mColour;};

	void SetSpreadAngle(float ang) {mSpreadAngle = ang; UpdateLightSprite();};
	float GetSpreadAngle() {return mSpreadAngle;};

	void SetAngle(float ang) {mAngle = ang;};
	float GetAngle() {return mAngle;};
};

