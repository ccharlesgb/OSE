#include "Lighting.h"


Lighting::Lighting(void)
{
	sf::Image BlackImg;
	BlackImg.create(1,1, sf::Color::Black);
	mBlackTex.loadFromImage(BlackImg);
}

Lighting::~Lighting(void)
{
}
