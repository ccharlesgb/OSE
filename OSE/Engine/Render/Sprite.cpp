#include "Sprite.h"
#include "../Resource.h"

Sprite::Sprite(sf::RenderWindow *rend) : BaseDrawable(rend)
{
	mShader= new sf::Shader();
	mShader->loadFromFile("shaders/blend.frag", sf::Shader::Fragment);
}

Sprite::~Sprite(void)
{
}

void Sprite::Draw()
{
	sf::RenderStates state;
	state.shader = mShader;
	mShader->setParameter("pixel_threshold", 0.02f);
	if (mDirtyTransform)
	{
		mSprite.setPosition(GameToSFML(GetPosition()).SF());
		mSprite.setRotation(-GetAngle());
		//mSprite.setTexture(mTexture);
		mSprite.setOrigin((GetOrigin() + GetTextureCentre()).SF());
		mSprite.setScale(sf::Vector2f(GetScale(),GetScale()));
		mDirtyTransform = false;
	}
	mRenderWindow->draw(mSprite, state);
}

void Sprite::SetTexture(const char* path)
{
	mSprite.setTexture(*Resource::RequestImage(path));
	SetPosition(Vector2(0,0));
	SetOrigin(GetTextureCentre()); //By default origin at middle of texture
}