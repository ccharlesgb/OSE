#include "Primitive.h"
#include "../Engine/GameGlobals.h"

LINKCLASSTONAME("primitive", Primitive)

Primitive::Primitive(void)
{
	RenderInit();
	SetDrawOrder(RENDERGROUP_BACKGROUND);
	mTexture.loadFromFile("images/paintedmud.png");

	PhysicsInit(STATIC_BODY);
	mVertexArray.setPrimitiveType(sf::TrianglesFan);

}


Primitive::~Primitive(void)
{
}

void Primitive::AddVertex(Vector2 vertex)
{
	std::cout << "Add Vertex: " << vertex.ToString() << "\n";
	if (vertex.x > mUpperBound.x)
		mUpperBound.x = vertex.x;
	if (vertex.y > mUpperBound.y)
		mUpperBound.y = vertex.y;
	if (vertex.x < mLowerBound.x)
		mLowerBound.x = vertex.x;
	if (vertex.y < mLowerBound.y)
		mLowerBound.y = vertex.y;
	mVertices.push_back(vertex);
}

void Primitive::CompileShape()
{
	std::cout << "LOWERBOUND: " << mLowerBound.ToString() << "\n";
	std::cout << "UPPERBOUND: " << mUpperBound.ToString() << "\n";
	SetRenderBounds(Vector2_Rect(mLowerBound, mUpperBound - mLowerBound));
	PolygonShape mHull;
	mHull.mDensity = 5.f;
	mHull.mIsChain = true;

	std::vector<Vector2>::iterator iter;
	iter = mVertices.begin();
	while (iter != mVertices.end())
	{
		Vector2 CurPos = *iter;
		Vector2 CurPosActual = CurPos;
		CurPosActual.y *= -1;
		sf::Vertex vert;
		vert.position = ig::GameToSFML(CurPosActual + GetPos()).SF();
		vert.texCoords = (mUpperBound - CurPos).SF();
		mVertexArray.append(vert);
		mHull.AddVertex(CurPosActual);
		iter++;
	}
	GetPhysObj()->AddPhysicsShape(&mHull);
}

void Primitive::Spawn()
{
	SetPos(Vector2(0,200));
	for (int i = 0; i < 10; i++)
	{
		float x,y;
		float circle_progress = 6.28f * ((float)i /  10.f);

		x = std::cos(circle_progress)  * ig::Random(50,200);
		y = std::sin(circle_progress)  * ig::Random(50,200);

		AddVertex(Vector2(x,y));
	}
	CompileShape();

}

void Primitive::Draw()
{
	sf::RenderStates state;
	state.texture = &mTexture;
	gGlobals.RenderWindow->draw(mVertexArray, state);
}
