#include "Text.h"
#include "Resource.h"
#include <SFML/System/String.hpp>

Font::Font(const char *path)
{
	mFont = *Resource::RequestFont(path);
};

Font::Font(sf::Font font)
{
	mFont = font;
};

Font::~Font()
{

}

Font Font::GetDefaultFont()
{
	return Font(sf::Font::getDefaultFont());
};

Text::Text(const std::string text, Font font = Font::GetDefaultFont(), unsigned int size = 30)
{
	mText = sf::Text(sf::String(text), font.mFont, size);
};
