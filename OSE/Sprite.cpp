#include "Sprite.h"


Sprite::Sprite(sf::RenderWindow *rend) : BaseDrawable(rend)
{
}

Sprite::~Sprite(void)
{
}

void Sprite::Draw()
{
	mSprite.setPosition(GameToSFML(GetPosition()).SF());
	mSprite.setRotation(-GetAngle());
	mRenderWindow->draw(mSprite);
}

void Sprite::SetTexture(const char* path)
{
	std::string FILE_PATH = GetImagePath(path);
	if (!mTexture.loadFromFile(FILE_PATH))
	{
		std::cout << "ERROR COULDN'T FIND SPRITE\n";
	}
	else
	{
		mSprite.setTexture(mTexture);
		mSprite.setPosition(0,0);
		mSprite.setOrigin(mTexture.getSize().x / 2, mTexture.getSize().y / 2);
	}
}