#include "BlendSprite.h"
#include "../Resource.h"
#include "../GameGlobals.h"

BlendSprite::BlendSprite(sf::RenderWindow *rend) : BaseDrawable(rend)
{
	mShader= new sf::Shader();
	mShader->loadFromFile("shaders/blend.frag", sf::Shader::Fragment);

	mBlendTexture.loadFromFile("images/blend_channel.png");
}

BlendSprite::~BlendSprite(void)
{
	delete mShader;
}

void BlendSprite::Draw()
{
	sf::RenderStates state;
	state.shader = mShader;
	mShader->setParameter("blend_channel", mBlendTexture);
	mShader->setParameter("other_texture", *mSprite2.getTexture());

	int blend_scale = 8;
	Vector2 GridPos = ig::NearestGrid(GetPosition(), 1024, 1024);
	GridPos.y *= -1;
	Vector2 Offset;

	if (GridPos.x > -0.02)
	{
		Offset.x = std::abs((int)GridPos.x) % blend_scale;
	}
	else
	{
		Offset.x = blend_scale - (std::abs((int)GridPos.x) % blend_scale);
		if (Offset.x == blend_scale)
			Offset.x = 0;
	}
	if (GridPos.y > -0.02)
	{
		Offset.y = std::abs((int)GridPos.y) % blend_scale;
	}
	else
	{
		Offset.y = blend_scale - (std::abs((int)GridPos.y) % blend_scale);
		if (Offset.y == blend_scale)
			Offset.y = 0;
	}
	
	Offset = Offset / blend_scale;

	mShader->setParameter("scale", blend_scale);
	mShader->setParameter("pos_x", Offset.x);
	mShader->setParameter("pos_y", Offset.y);
	//std::cout << GridPos.ToString() << "\n";
	if (mDirtyTransform)
	{
		mSprite1.setPosition(GameToSFML(GetPosition()).SF());
		mSprite1.setRotation(-GetAngle());
		mSprite1.setOrigin((GetOrigin() + GetTextureCentre()).SF());
		mSprite1.setScale(sf::Vector2f(GetScale(),GetScale()));

		mSprite2.setPosition(GameToSFML(GetPosition()).SF());
		mSprite2.setRotation(-GetAngle());
		mSprite2.setOrigin((GetOrigin() + GetTextureCentre()).SF());
		mSprite2.setScale(sf::Vector2f(GetScale(),GetScale()));
		mDirtyTransform = false;
	}
	mRenderWindow->draw(mSprite1,state);
}

void BlendSprite::SetTexture1(const char* path)
{
	mSprite1.setTexture(*TextureResource::GetTexture(path));
	//mSprite1.getTexture()->setSmooth(true);
	SetPosition(Vector2(0,0));
}

void BlendSprite::SetTexture2(const char* path)
{
	mSprite2.setTexture(*TextureResource::GetTexture(path));
	SetPosition(Vector2(0,0));
}