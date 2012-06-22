#pragma once

#include <SFML/Graphics.hpp>
#include "../Engine/BaseDrawable.h"

class Line : public BaseDrawable
{
private:
	sf::VertexArray* mLine;
	Colour mCol;
public:
	Vector2 mVerts[2];
	Line(sf::RenderWindow *rend);
	~Line(void);
	void Draw();

	void SetColour(const Colour col) {mCol = col;};
	Colour GetColour() {return mCol;};
};

