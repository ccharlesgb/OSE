#include "Sprite.h"


Sprite::Sprite(sf::RenderWindow *rend) : BaseDrawable(rend)
{

}

Sprite::~Sprite(void)
{
}

void Sprite::Draw()
{
	if (mDirtyTransform)
	{
		mSprite.setPosition(GameToSFML(GetPosition()).SF());
		mSprite.setRotation(-GetAngle());
		mSprite.setTexture(mTexture);
		mSprite.setOrigin((GetOrigin() + GetTextureCentre()).SF());
		mSprite.setScale(sf::Vector2f(GetScale(),GetScale()));
		mDirtyTransform = false;
	}
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
		SetPosition(Vector2(0,0));
		SetOrigin(GetTextureCentre()); //By default origin at middle of texture
	}
}