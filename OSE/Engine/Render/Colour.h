#pragma once

#include <SFML/Graphics.hpp>

class Colour
{
public:
	int r;
	int g;
	int b;
	int a;

	float h;
	float s;
	float v;
	Colour() {r=g=b=a=255;};
	Colour(int R, int G, int B) {r=R; g = G; b = B; a = 255;};
	Colour(int R, int G, int B, int A) {r=R; g = G; b = B; a = A;};
	Colour(const Colour& col) {r = col.r; g = col.g; b = col.b; a = col.a;};
	sf::Color SF() {return sf::Color(r, g, b, a);};

	void HSVToRGB();
};

inline void Colour::HSVToRGB()
{
#pragma warning( disable : 4244 )
    int hi = int(std::floor(h / 60.0)) % 6;
    float f = (h / 60.0) - std::floor(h / 60.0);
    float p = v * (1.0 - s);
    float q = v * (1.0 - (f*s));
	float t = v * (1.0 - ((1.0 - f) * s));
    switch (hi)
	{
		case 0:
			r = v; g = t; b = p;
			break;
		case 1:
			r = q; g = v; b= p;
			break;
		case 2:
			r = q; g = v; b= p;
			break;
		case 3: 
			r = p; g = q; b= v;
			break;
		case 4:
			r = t; g = p; b= v;
			break;
		case 5:
			r = v; g = p; b= q;
			break;
    }
#pragma warning( default : 4244 )
}