#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2.h"

class BaseDrawable
{
protected:
	Vector2 mPos;
	float mAngle;
	float mScale;
	sf::RenderWindow* mRenderWindow;
	bool mDirtyTransform;
public:
	BaseDrawable(sf::RenderWindow *rend);
	~BaseDrawable(void);

	Vector2 GameToSFML(Vector2 pos);

	void SetPosition(Vector2 pos) {mPos = pos; mDirtyTransform = true;};
	Vector2 GetPosition() {return mPos;};

	void SetAngle(float a) {mAngle = a; mDirtyTransform = true;};
	float GetAngle() {return mAngle;};

	void SetScale(float s) {mScale = s; mDirtyTransform = true;};
	float GetScale() {return mScale;};

	void SetRenderWindow(sf::RenderWindow* pRend) {mRenderWindow = pRend;};
	virtual void Draw() = 0;
};

