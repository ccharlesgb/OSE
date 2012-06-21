#pragma once

#include <SFML/Graphics.hpp>
#include "../Engine/BaseDrawable.h"

class Line : public BaseDrawable
{
private:
	sf::VertexArray* mLine;
	Vector2 mVerts[2];
public:
	Line(sf::RenderWindow *rend);
	~Line(void);
	void Draw();
};

