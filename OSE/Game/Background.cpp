#include "Background.h"
#include "Camera.h"
#include "GameGlobals.h"

LINKCLASSTONAME("background", Background)

#define METEOR_SPEED 20

Background::Background(void)
{
	SetPos(Vector2(-TEXTURE_SIZE / 2.f,TEXTURE_SIZE / 2.f));
	SetRenderType(RENDER_CUSTOM);
	SetRenderer(new BGRender);
	InitRenderer();
	GetRenderer()->SetDrawOrder(RENDERGROUP_BACKGROUND);
}

Background::~Background(void)
{
}

void Background::Think()
{

}

void Background::Spawn()
{

}

BGRender::BGRender()
{
	ARR_SIZES[0] = TEXTURE_SIZE / 10.f;
	ARR_SIZES[1] = TEXTURE_SIZE / 30.f;
	ARR_SIZES[2] = TEXTURE_SIZE / 55.f;
	EnableVisChecks(false); //Dont cull!
	mNextMeteor = gGlobals.CurTime;
	mMeteor = Vector2(0,0);
	mDrawOrder = 0;
	mDrawingMeteor = false;
	int tex_size = TEXTURE_SIZE;
	if (!mStarTex.loadFromFile("images/star.png"))
	{
		std::cout << "Invalid filename: " << "star" << "\n";
	}
	if (!mMeteorTex.loadFromFile("images/sun.png"))
	{
		std::cout << "Invalid filename: " << "sun" << "\n";
	}
	mStarTex.setSmooth(true);
	mStar.setTexture(mStarTex);
	mMeteorSprite.setTexture(mStarTex);
	mMeteorSprite.setOrigin(256,256);

	float StarTexWidth = (float)mStarTex.getSize().x;
	for (int pass=0; pass < MAX_PASSES; pass++)
	{
		mTexture[pass].create(tex_size,tex_size);
		mTexture[pass].setSmooth(true);
		int arr_size = ARR_SIZES[pass];
		float pos_scale = (float)tex_size / (float)arr_size;
		float scale = ((float)pos_scale / StarTexWidth) / 1.4f;
		float randomness = pos_scale / 2.f;
		std::cout << "GENERATING PASS #" << pass << "--------------\n";
		std::cout << "Array Size: " << arr_size << "\n";
		std::cout << "POS SCALE: " << pos_scale << "\n";
		std::cout << "Star Scale: " << scale << "\n";
		for (int x=0; x < arr_size; x++)
		{
			for (int y=0; y < arr_size; y++)
			{
				float individual_scale = scale;// + (ig::Random(-0.1,0.1) * 0.1 * pass);
				Vector2 pos = Vector2(x,y) * pos_scale;
				pos = pos + Vector2::Random(-randomness,randomness);
				mStar.setScale(individual_scale, individual_scale);
				sf::Color col = sf::Color(ig::Random(190,255),ig::Random(180,200),ig::Random(190,255));
				mStar.setPosition(pos.SF());
				mStar.setRotation(ig::Random(0,360));
				mStar.setColor(col);
				mTexture[pass].draw(mStar);
			}
		}
		mTexture[pass].display();
		mSprite[pass].setTexture(mTexture[pass].getTexture());
		mSprite[pass].setPosition(0,0);
		mSprite[pass].setOrigin(TEXTURE_SIZE / 2, TEXTURE_SIZE / 2);
		mSprite[pass].setScale(TEXTURE_SCALE,TEXTURE_SCALE);
	}
}

void BGRender::PreDraw(sf::RenderWindow *pRender)
{	

}

void BGRender::Draw(sf::RenderWindow *pRender)
{
	float zoom_level = sCamera::GetZoom();
	for (int i=0; i < MAX_PASSES; i++)
	{
		Vector2 plyPos = gGlobals.Player->GetPos();
		float DepthFactor = ((i + 1.f) * 0.1f) + 1.5f;
		Vector2 DepthOffset = plyPos / DepthFactor;
		Vector2 QuadPos = plyPos - DepthOffset;
		Vector2 Quadrant; //Quantise the player position a grid size TEXTURE_SIZE
		Quadrant.x = std::floor((QuadPos.x / (TEXTURE_SIZE_TRUE - SLOP) + 0.5f)) - 0.5f;
		Quadrant.y = std::floor((QuadPos.y / (TEXTURE_SIZE_TRUE - SLOP) + 0.5f)) - 0.5f;
		sf::Color col = mSprite[i].getColor();
		float alpha = 255 - (ARR_SIZES[i] * ARR_SIZES[i] * (zoom_level - 1) * 0.0015);
		//float alpha = 255 - ((1.f/zoom_level - 1.f) * (MAX_PASSES - i) * (MAX_PASSES - i) * 12.f);
		alpha = ig::Limit(alpha, 0, 255);
		mSprite[i].setColor(sf::Color(col.r,col.g,col.b, alpha));
		for (int x=-1; x < 3; x++)
		{
			for (int y=-1; y < 3; y++)
			{
				Vector2 Offset = Vector2((x) * (TEXTURE_SIZE - SLOP) * TEXTURE_SCALE, (y) * (TEXTURE_SIZE - SLOP) * TEXTURE_SCALE);
				Vector2 RenderPos = DepthOffset + (Quadrant * TEXTURE_SIZE_TRUE) + Offset;
				sf::Vector2f RenderPosSF = GameToSFML(RenderPos, pRender);
				mSprite[i].setPosition(RenderPosSF);
				pRender->draw(mSprite[i]);
				//std::cout << "PASS: " << i << " ALPHA: " << alpha << "\n";
			}
		}
	}

	//Hacky code for a shooting star but it looks nice!
	if (mDrawingMeteor)
	{
		mMeteor = mMeteor + Vector2(METEOR_SPEED / zoom_level,METEOR_SPEED / zoom_level);
		Vector2 DrawPos = pRender->convertCoords(sf::Vector2i(mMeteor.x, mMeteor.y));

		float met_scale = (zoom_level * 0.1) - (zoom_level * zoom_level * 0.005);
		met_scale = ig::Limit(met_scale,0,0.5);
		mMeteorSprite.setColor(sf::Color(255,230,200,255));
		//Draw Trail
		int passes = 10;
		for (int i=passes - 1; i >= 0; i--)
		{
			float scale = (((float)passes-(float)i) / (float)passes) * met_scale;
			DrawPos = DrawPos + Vector2(scale * 40, scale * 40);
			sf::Color col = mMeteorSprite.getColor();
			float alpha = 255 - ((i / passes) * 230);
			mMeteorSprite.setScale(Vector2(scale,scale).SF());
			mMeteorSprite.setColor(sf::Color(col.r,col.g,col.b, alpha));
			mMeteorSprite.setPosition(DrawPos.SF());
			pRender->draw(mMeteorSprite);
		}
		if (mMeteor.x > 1000)
		{
			mNextMeteor = gGlobals.CurTime + ig::Random(5,15);
			mDrawingMeteor = false;
		}
	}
	else
	{
		if (mNextMeteor < gGlobals.CurTime)
		{
			mMeteor = Vector2(ig::Random(0,500),1);
			mDrawingMeteor = true;
		}
	}
}

BGRender::~BGRender()
{

}