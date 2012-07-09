#include "BackgroundRender.h"
#include "../Resource.h"
#include "../GameGlobals.h"

#pragma warning( disable : 4244 )

BlendSprite::BlendSprite(sf::RenderWindow *rend) : BaseDrawable(rend)
{
	mShader= new sf::Shader();
	mShader->loadFromFile("shaders/blend.frag", sf::Shader::Fragment);

	mQuads.setPrimitiveType(sf::Quads);
}

BlendSprite::~BlendSprite(void)
{
	delete mShader;
}

void BlendSprite::Draw()
{
	sf::RenderStates state;
	state.shader = mShader;
	for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++)
	{
		QuadInfo CurInfo = mQuadInfos[i];
		//mSprite.setTexture(
		//mShader->setParameter("blend_channel", mBlendTexture);
		//mShader->setParameter("other_texture", *mSprite2.getTexture());

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
		
		}
		mRenderWindow->draw(mQuads,state);
	}
}

void BlendSprite::SetTexture1(int GridX, int GridY, const char* path)
{
	if (GridX >= MAP_WIDTH || GridY >= MAP_HEIGHT)
		return;
	int array_pos = GridX + (GridY * MAP_WIDTH);
	mQuadInfos[array_pos].mTex1 = TextureResource::GetTexture(path);
}

void BlendSprite::SetTexture2(int GridX, int GridY, const char* path)
{
	if (GridX >= MAP_WIDTH || GridY >= MAP_HEIGHT)
		return;
	int array_pos = GridX + (GridY * MAP_WIDTH);
	mQuadInfos[array_pos].mTex1 = TextureResource::GetTexture(path);
}