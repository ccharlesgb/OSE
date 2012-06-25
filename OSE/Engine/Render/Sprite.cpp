#include "Sprite.h"
#include "../Resource.h"

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
		//mSprite.setTexture(mTexture);
		mSprite.setOrigin((GetOrigin() + GetTextureCentre()).SF());
		mSprite.setScale(sf::Vector2f(GetScale(),GetScale()));
		mDirtyTransform = false;
	}
	mRenderWindow->draw(mSprite);
}

void Sprite::SetTexture(const char* path)
{
	mSprite.setTexture(*TextureResource::GetTexture(path));
	SetPosition(Vector2(0,0));
	SetOrigin(GetTextureCentre()); //By default origin at middle of texture
}