#include "Line.h"


Line::Line(sf::RenderWindow *rend) : BaseDrawable(rend)
{
	mLine = new sf::VertexArray(sf::LinesStrip,2);
}

Line::~Line(void)
{
	delete mLine;
}

void Line::Draw()
{
	if (mDirtyTransform)
	{
		
	}
	(*mLine)[0].position = mVerts[0].SF();
	(*mLine)[1].position = mVerts[1].SF();
	mRenderWindow->draw(*mLine);
}

