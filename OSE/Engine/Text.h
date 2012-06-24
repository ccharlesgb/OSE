#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2.h"

/**
 * Wrapper class for sf::Font.
 */
class Font
{
private:
	sf::Font mFont;
public:
	Font(const char *path);
	Font(sf::Font font);
	~Font();
	
	static Font GetDefaultFont();
};

/**
 * Wrapper class for sf::Text
 */
class Text
{
private:
	sf::Text mText;
public:
	enum Style {
		Regular = 0,
		Bold = 1 << 0,
		Italic = 1 << 1,
		Underlined = 1 << 2
	};
	
	Text(const std::string text, Font font, unsigned int size);
	~Text();
	
	void SetText(const std::string text) { mText.setString(text); };
	std::string GetText() { return mText.getString().toAnsiString(); };
	
	void SetPosition(const Vector2 position);
};