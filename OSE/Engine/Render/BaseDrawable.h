#pragma once

#include <SFML/Graphics.hpp>
#include "../Utilities/Vector2.h"
#include "Colour.h"

class BaseDrawable
{
protected:
	Vector2 mPos;
	Vector2 mOrigin;
	float mAngle;
	float mScale;
	sf::RenderWindow* mRenderWindow;
	bool mDirtyTransform;
	bool mUseScreenCoords;
public:
	BaseDrawable(sf::RenderWindow *rend);
	~BaseDrawable(void);

	Vector2 GameToSFML(Vector2 pos);

	std::string GetImagePath(const char* path);

	void SetUseScreenCoords(bool screencoords) {mUseScreenCoords = screencoords;};

	void SetPosition(Vector2 pos) {mPos = pos; mDirtyTransform = true;};
	Vector2 GetPosition() {return mPos;};

	void SetOrigin(Vector2 orig) {mOrigin = orig; mDirtyTransform = true;};
	Vector2 GetOrigin() {return mOrigin;};

	void SetAngle(float a) {mAngle = a; mDirtyTransform = true;};
	float GetAngle() {return mAngle;};

	void SetScale(float s) {mScale = s; mDirtyTransform = true;};
	float GetScale() {return mScale;};

	void SetRenderWindow(sf::RenderWindow* pRend) {mRenderWindow = pRend;};
	virtual void Draw() = 0;
};

