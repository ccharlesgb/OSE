#pragma once

#include <SFML/Graphics.hpp>

class Colour
{
public:
	int r;
	int g;
	int b;
	int a;
	Colour() {r=g=b=a=255;};
	Colour(int R, int G, int B) {r=R; g = G; b = B; a = 255;};
	Colour(int R, int G, int B, int A) {r=R; g = G; b = B; a = A;};
	Colour(const Colour& col) {r = col.r; g = col.g; b = col.b; a = col.a;};
	sf::Color SF() {return sf::Color(r, g, b, a);};
};