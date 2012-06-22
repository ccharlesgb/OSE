#pragma once

#include <SFML/Graphics.hpp>
#include "Utilities/Vector2.h"

class Colour
{
public:
	int r;
	int g;
	int b;
	int a;
	Colour() {r=g=b=a=255;};
	Colour(int R, int G, int B) {r=R; g = G; b = B; a = 255;};
	Colour(int R, int G, int B, int A) {r=R; g = G; b = B; a = A;};
	Colour(const Colour& col) {r = col.r; g = col.g; b = col.b; a = col.a;};
	sf::Color SF() {return sf::Color(r, g, b, a);};
};

class BaseDrawable
{
protected:
	Vector2 mPos;
	Vector2 mOrigin;
	float mAngle;
	float mScale;
	sf::RenderWindow* mRenderWindow;
	bool mDirtyTransform;
public:
	BaseDrawable(sf::RenderWindow *rend);
	~BaseDrawable(void);

	Vector2 GameToSFML(Vector2 pos);

	std::string GetImagePath(const char* path);

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

