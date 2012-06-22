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
	(*mLine)[0].color = GetColour().SF();
	(*mLine)[1].color = GetColour().SF();
	(*mLine)[0].position = ig::GameToSFML(mVerts[0]).SF();
	(*mLine)[1].position = ig::GameToSFML(mVerts[1]).SF();
	mRenderWindow->draw(*mLine);
}

