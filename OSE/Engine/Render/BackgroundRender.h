#pragma once

#include <SFML/Graphics.hpp>
#include "BaseDrawable.h"

struct QuadInfo
{
	sf::Texture* mTex1;
	sf::Texture* mTex2;
	sf::Texture* mBlendTexture;
};

#define MAP_WIDTH 8
#define MAP_HEIGHT 8

class BlendSprite : public BaseDrawable
{
private:
	sf::Shader* mShader;
	sf::Sprite mSprite;
	sf::VertexArray mQuads;

	QuadInfo mQuadInfos[MAP_WIDTH * MAP_HEIGHT];
public:
	BlendSprite(sf::RenderWindow *rend);
	~BlendSprite(void);
	Vector2 GetSize()
	{
		Vector2 size;
		size.x = 1024.f;
		size.y = 1024.f;
		return size;
	};
	Vector2 GetTextureCentre() {return Vector2();};
	void Draw();
	void SetTexture1(int GridX, int GridY, const char* path);
	void SetTexture2(int GridX, int GridY, const char* path);
	void SetColour(Colour col) {};
};

