#include "Text.h"
#include "Resource.h"


Text::Text(std::string text, Font font = Font::GetDefaultFont())
{
	
};


Font::Font(const char *path)
{
	mFont = *Resource::RequestFont(path);
};

Font::Font(sf::Font font)
{
	mFont = font;
};

Font Font::GetDefaultFont()
{
	return Font(sf::Font::getDefaultFont());
};