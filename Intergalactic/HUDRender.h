#pragma once

#include "BaseRender.h"
#include <string>
#include <SFML/Graphics.hpp>

class HUDRender
{
private:
	sf::RenderTexture mRenderTexture;
	sf::Text mTextFrameTime;
	sf::Shape *mPlyInd;
	std::map<std::string, sf::Drawable*> mElements;
	sf::RenderWindow *mRend;
	sf::Text mText;
	sf::Shape *mCircle;
public:
	void AddTextElement(const char* ID);
	sf::Text* GetTextElement(const char*ID);

	void AddShapeElement(const char* ID);
	sf::Shape* GetShapeElement(const char*ID);
	void Draw(sf::RenderWindow *rend);
	
	void DrawText(const char* txt, Vector2 pos, int size, sf::Color col);
	//void DrawShape(Vector2 pos, float radius, sf::Color col, float outline, sf::Color out_col);

	HUDRender(void);
	~HUDRender(void);
};

