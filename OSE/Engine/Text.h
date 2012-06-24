#pragma once

#include <SFML/Graphics.hpp>

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
	
	Text(std::string text, Font font);
	~Text();
};