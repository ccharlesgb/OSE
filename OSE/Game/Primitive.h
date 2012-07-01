#pragma once

#include "../Engine/Bases/BasePhysics.h"
#include <SFML/Graphics.hpp>

class Primitive : public BasePhysics
{
private:
	std::vector<Vector2> mVertices;
	sf::VertexArray mVertexArray;
	sf::Texture mTexture;
	Vector2 mUpperBound;
	Vector2 mLowerBound;
public:
	Primitive(void);
	~Primitive(void);

	void Spawn();
	void Draw();

	void AddVertex(Vector2 vertex);
	void CompileShape();
};

