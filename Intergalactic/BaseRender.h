#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include "MathUtils.h"
#include "Vector2.h"

#define RENDER_SCALE 1.f

enum RenderGroup
{
	RENDERGROUP_BACKGROUND, //Draw first
	RENDERGROUP_ENTITIES,
	RENDERGROUP_PLAYER,
	RENDERGROUP_HUD //Draw Last
};

class BaseRender
{
private:
	BaseRender(const BaseRender&);
	BaseRender& operator=(const BaseRender&);
	bool mDoVisChecks;
protected:
	int mDrawOrder;
	Vector2 mPos;
	float mAngle;
	//sf::RenderWindow *pRender;
	sf::Color mColour;
public:
	void SetPos(Vector2 p) {mPos = p;};
	sf::Vector2f GameToSFML(Vector2 Pos, sf::RenderWindow *pRender);
	sf::Vector2f RenderPos(sf::RenderWindow *pRender) {return GameToSFML(mPos, pRender);};
	
	Vector2 ToScreen(Vector2 pos);

	BaseRender();
	virtual sf::Drawable* GetDrawable() = 0;
	virtual sf::IntRect GetBB() {std::cout << "NO BOUNDING BOX SPECIFIED ON RENDERER\n"; return sf::IntRect();};
	virtual void PreDraw(sf::RenderWindow *pRender);
	virtual void Draw(sf::RenderWindow *pRender) {pRender->Draw(*GetDrawable());};

	void EnableVisChecks(bool e) {mDoVisChecks = e;};
	bool GetEnableVisChecks() {return mDoVisChecks;};

	void SetScale(Vector2 scale) {GetDrawable()->SetScale(scale.SF());};
	virtual void SetSize(Vector2 size) {};

	void SetAngle(float ang) {mAngle = ang;};
	float GetAngle() {return mAngle;};

	virtual void SetColour(sf::Color col) = 0;
	sf::Color GetColour() {return mColour;};
	int GetDrawOrder() {return mDrawOrder;};
	void SetDrawOrder(int priority) {mDrawOrder = priority;};

	//Shape
	virtual void Circle(float Radius, sf::Color Colour, float Outline, sf::Color OutlineColour) {};
	virtual void Rectangle(Vector2 min, Vector2 max, sf::Color Colour, float Outline, sf::Color OutlineColour) {};
	virtual void Polygon(Vector2* vertices, int vertexcount, sf::Color Colour, float Outline, sf::Color OutlineColour) {};
	virtual void SetOutlineColour(sf::Color col) {};
	virtual void SetOutlineWeight(float w) {};

	//Sprite
	virtual void SetTexture(const char* FileName, bool Smooth) {};
	virtual void SetSmoothed(bool s) {};
};

class ShapeRender : public BaseRender
{
private:
	sf::Shape mShape;
public:
	ShapeRender();

	sf::Drawable *GetDrawable()
	{
		return &mShape;
	}
	sf::IntRect GetBB()
	{
		sf::Vector2f TL, TR, BL, BR;
		TL = TR = BL = BR = mShape.TransformToGlobal(mShape.GetPointPosition(0));
		for (unsigned int i=0; i < mShape.GetPointsCount(); i++)
		{
			sf::Vector2f point = mShape.TransformToGlobal(mShape.GetPointPosition(i));

			TL.x = std::min(point.x, TL.x);
			TL.y = std::max(point.y, TL.y);

			TR.x = std::max(point.x, TR.x);
			TR.y = std::max(point.y, TR.y);

			BL.x = std::min(point.x, BL.x);
			BL.y = std::min(point.y, BL.y);

			BR.x = std::max(point.x, BR.x);
			BR.y = std::min(point.y, BR.y);
		}
		TL.x -= 1;
		TL.y -= 1;
		return sf::IntRect((int)TL.x, (int)TL.y, BR.x - TL.x + 1, TL.y - BR.y + 1);
	}
	void Circle(float Radius, sf::Color Colour, float Outline, sf::Color OutlineColour);
	void Rectangle(Vector2 min, Vector2 max, sf::Color Colour, float Outline, sf::Color OutlineColour);
	void Polygon(Vector2* vertices, int vertexcount, sf::Color Colour, float Outline, sf::Color OutlineColour);
	void SetOutlineColour(sf::Color col) 
	{
		for (unsigned int i=1; i <= mShape.GetPointsCount(); i++)
			mShape.SetPointOutlineColor(i, col);
	};
	void SetOutlineWeight(float w) {mShape.SetOutlineThickness(w);};
	void SetColour(sf::Color col)
	{
		mColour = col;
		for (unsigned int i=0; i < mShape.GetPointsCount(); i++)
			mShape.SetPointColor(i, col);
	};
};

class SpriteRender : public BaseRender
{
private:
	sf::Sprite mSprite;
	sf::Texture mTex;
public:
	SpriteRender();
	sf::IntRect GetBB()
	{
		std::cout << mSprite.GetSubRect().Width << "\n";
		return mSprite.GetSubRect();
	}
	sf::Drawable *GetDrawable()
	{
		return &mSprite;
	}
	void SetSize(Vector2 size) {mSprite.Resize(size.SF() * RENDER_SCALE);};
	void SetColour(sf::Color col) {mSprite.SetColor(col); mColour = col;};
	void SetTexture(const char* FileName, bool Smooth);
};

class TextRender : public BaseRender
{
private:
	sf::Text mText;
public:
	TextRender();

	sf::Drawable *GetDrawable()
	{
		return &mText;
	}
	void SetColour(sf::Color col) {mText.SetColor(col); mColour = col;};
	void SetText(std::string &txt) {mText.SetString(txt);};
	std::string GetText() {return mText.GetString();};
};