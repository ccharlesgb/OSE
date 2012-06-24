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
	/**
	 * Drawing styles.
	 */
	enum Style
    {
		Regular    = 0,      ///< Regular characters, no style
		Bold       = 1 << 0, ///< Bold characters
		Italic     = 1 << 1, ///< Italic characters
		Underlined = 1 << 2  ///< Underlined characters
    };
	
	/**
	 * Default constructor
	 *
	 * @param sf::RenderWindow Render window
	 */
	Text(sf::RenderWindow *rend);
	~Text();
	
	/**
	 * Draw the text to render window.
	 */
	void Draw();
	
	/**
	 * Set the text's string.
	 *
	 * @param string Text
	 * @see GetText
	 */
	void SetText(const std::string text) { mText.setString(text); };
	
	/**
	 * Get the text's string.
	 *
	 * @see SetText
	 */
	std::string GetText() { return mText.getString().toAnsiString(); };
	
	/**
	 * Set the text's position.
	 *
	 * @param Vector2 New position
	 */
	void SetPosition(const Vector2 position);
	
};