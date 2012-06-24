#pragma once

#include <SFML/Graphics.hpp>
#include "Render/BaseDrawable.h"
#include "Utilities/Vector2.h"

/**
 * Wrapper class for sf::Font.
 *
 * @ingroup engine
 */
class Font
{
private:
	sf::Font mFont;
public:
	friend class Text;
	Font(const char *path);
	Font(sf::Font font);
	~Font();
	
	static Font GetDefaultFont();
};

/**
 * Wrapper class for sf::Text
 *
 * @ingroup engine
 */
class Text : public BaseDrawable
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
	
	Text(sf::RenderWindow *rend);
	~Text();
	
	void Draw();
	
	void SetText(const std::string text) { mText.setString(text); };
	std::string GetText() { return mText.getString().toAnsiString(); };
	
	void SetPosition(const Vector2 position);
	
	friend class Font;
};