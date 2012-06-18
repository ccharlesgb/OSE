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
	virtual sf::FloatRect GetBB() {std::cout << "NO BOUNDING BOX SPECIFIED ON RENDERER\n"; return sf::FloatRect();};
	virtual void PreDraw(sf::RenderWindow *pRender);
	virtual void Draw(sf::RenderWindow *pRender) {pRender->draw(*GetDrawable());};

	void EnableVisChecks(bool e) {mDoVisChecks = e;};
	bool GetEnableVisChecks() {return mDoVisChecks;};

	void SetScale(Vector2 scale) {};
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
	sf::Shape *mShape;
public:
	ShapeRender();

	sf::Drawable *GetDrawable()
	{
		return mShape;
	}
	sf::FloatRect GetBB()
	{
		return mShape->getGlobalBounds();
	}
	void Circle(float Radius, sf::Color Colour, float Outline, sf::Color OutlineColour);
	void Rectangle(Vector2 min, Vector2 max, sf::Color Colour, float Outline, sf::Color OutlineColour);
	void Polygon(Vector2* vertices, int vertexcount, sf::Color Colour, float Outline, sf::Color OutlineColour);
	void SetOutlineColour(sf::Color col) 
	{
		mShape->setOutlineColor(col);
	};
	void SetOutlineWeight(float w) {mShape->setOutlineThickness(w);};
	void SetColour(sf::Color col)
	{
		mColour = col;
		mShape->setFillColor(col);
	};
};

class SpriteRender : public BaseRender
{
private:
	sf::Sprite mSprite;
	sf::Texture mTex;
public:
	SpriteRender();
	sf::FloatRect GetBB()
	{
		return mSprite.getGlobalBounds();
	}
	sf::Drawable *GetDrawable()
	{
		return &mSprite;
	}
	void SetSize(Vector2 size) {};//TODO:: IMPLEMENT THIS
	void SetColour(sf::Color col) {mSprite.setColor(col); mColour = col;};
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
	void SetColour(sf::Color col) {mText.setColor(col); mColour = col;};
	void SetText(std::string &txt) {mText.setString(txt);};
	std::string GetText() {return mText.getString();};
};