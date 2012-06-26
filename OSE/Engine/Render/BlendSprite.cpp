#include "BlendSprite.h"
#include "../Resource.h"

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
	Vector2 GridPos = ig::NearestGrid(GetPosition(), 1024,1024);
	GridPos.x = std::fmod(GridPos.x, 2) + 1;
	GridPos.y = std::fmod(GridPos.y, 2) + 1;;
	//std::cout << GridPos.ToString() << "\n";
	mShader->setParameter("pos_x", GridPos.x);
	mShader->setParameter("pos_y", GridPos.y);
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
	SetOrigin(GetTextureCentre()); //By default origin at middle of texture
}

void BlendSprite::SetTexture2(const char* path)
{
	mSprite2.setTexture(*TextureResource::GetTexture(path));
	SetPosition(Vector2(0,0));
	SetOrigin(GetTextureCentre()); //By default origin at middle of texture
}