#pragma once

#include <SFML/Graphics.hpp>
#include "BaseDrawable.h"

class BlendSprite : public BaseDrawable
{
private:
	sf::Sprite mSprite1;
	sf::Sprite mSprite2;
	sf::Shader *mShader;
	sf::Texture mBlendTexture;
public:
	BlendSprite(sf::RenderWindow *rend);
	~BlendSprite(void);
	Vector2 GetSize()
	{
		Vector2 size;
		size.x = (float)mSprite1.getTexture()->getSize().x;
		size.y = (float)mSprite1.getTexture()->getSize().y;
		return size;
	};
	Vector2 GetTextureCentre() {return Vector2(mSprite1.getTexture()->getSize().x / 2, mSprite1.getTexture()->getSize().y / 2);};
	void Draw();
	void SetTexture1(const char* path);
	void SetTexture2(const char* path);
	void SetColour(Colour col) {mSprite1.setColor(sf::Color(col.r, col.g,col.b,col.a));};
};

