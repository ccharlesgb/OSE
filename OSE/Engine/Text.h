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
	sf::Font mFont; ///< sf::Font to use.
public:
	/**
	 * Default constructor.
	 *
	 * @param char Path to font
	 */
	Font(const char *path);
	~Font();
	
	friend class Text;
};

/**
 * Wrapper class for sf::Text
 *
 * @ingroup engine
 */
class Text : public BaseDrawable
{
private:
	sf::Text mText; ///< sf::Text to use.
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
	 * Set the text's font.
	 *
	 * @param char path
	 */
	void SetFont(const char *path);
	
	/**
	 * Set the text's string.
	 *
	 * @param string New text
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