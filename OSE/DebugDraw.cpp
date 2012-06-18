#include "DebugDraw.h" 
#include "Renderer.h"
#include "BasePhysics.h"

#pragma warning(disable:4244) //float to SF::Uint8

DebugDraw::DebugDraw(sf::RenderWindow *r) 
{ 
	pRender = r;
   SetFlags(e_shapeBit | e_jointBit | e_centerOfMassBit /*| e_aabbBit*/); 
   //SetFlags(0xFFFF); 
} 

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
{ 
   sf::ConvexShape Shape(vertexCount); 
   Shape.setOutlineThickness(1.f); 
   for (int32 i = 0; i < vertexCount; ++i) 
   { 
	  Vector2 pos = RENDERER->GameToSFML(Vector2(vertices[i]) * PIXELS_PER_METRE);
      Shape.setPoint(i, pos.SF());
   } 
	pRender->draw(Shape); 
} 
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
{ 
   sf::ConvexShape Shape(vertexCount);
   Shape.setOutlineThickness(1.f);
   for (int32 i = 0; i < vertexCount; ++i)
   {
	  Vector2 pos = RENDERER->GameToSFML(Vector2(vertices[i]) * PIXELS_PER_METRE);
      Shape.setPoint(i, sf::Vector2f(pos.x, pos.y)); 
   } 
   pRender->draw(Shape); 
} 
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) 
{ 
	Vector2 pos = RENDERER->GameToSFML(Vector2(center) * PIXELS_PER_METRE);
    sf::CircleShape Shape(radius, radius / 5.f);
   Shape.setFillColor(sf::Color(color.r*255, color.g*255, color.b*255));
   Shape.setPosition(pos.x, pos.y); 
   pRender->draw(Shape); 
} 
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) 
{ 
	Vector2 pos = RENDERER->GameToSFML(Vector2(center) * PIXELS_PER_METRE);
   sf::CircleShape Shape(radius, radius / 5.f);
   Shape.setFillColor(sf::Color(color.r*255, color.g*255, color.b*255));
   Shape.setPosition(pos.x, pos.y); 
   pRender->draw(Shape); 
} 
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) 
{ 
	//Vector2 pos1 = RENDERER->GameToSFML(p1);
	//Vector2 pos2 = RENDERER->GameToSFML(p2);
	//sf::
	//sf::Shape Line = sf::Shape::Line(pos1.x, pos1.y, pos2.x, pos2.y, 1.f, sf::Color(color.r*255, color.g*255, color.b*255)); 
	//pRender->Draw(Line); 
} 
void DebugDraw::DrawTransform(const b2Transform& xf) 
{ 
   //Log("Debug draw: Transform"); 
   //std::cout << "Drawing transform" << std::endl; 
   /*b2Vec2 p1 = xf.position, p2; 
   const float32 k_axisScale = 0.4f; 
   glBegin(GL_LINES); 
    
   glColor3f(1.0f, 0.0f, 0.0f); 
   glVertex2f(p1.x, p1.y); 
   p2 = p1 + k_axisScale * xf.R.col1; 
   glVertex2f(p2.x, p2.y); 

   glColor3f(0.0f, 1.0f, 0.0f); 
   glVertex2f(p1.x, p1.y); 
   p2 = p1 + k_axisScale * xf.R.col2; 
   glVertex2f(p2.x, p2.y); 

   glEnd();*/ 
} 
void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) 
{ 
	Vector2 pos = RENDERER->GameToSFML(p);
	sf::CircleShape shape(size, size / 5.f);
	shape.setPosition(sf::Vector2f(pos.x,pos.y));
	shape.setFillColor(sf::Color(color.r*255, color.g*255, color.b*255));
	pRender->draw(shape); 
} 
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color) 
{ 
	Vector2 lower = RENDERER->GameToSFML(aabb->lowerBound);
	Vector2 upper = RENDERER->GameToSFML(aabb->upperBound);
	sf::RectangleShape shape;
	shape.setSize((upper - lower).SF());
	shape.setPosition((lower + (shape.getSize() / 2.f)).SF());
	shape.setFillColor(sf::Color(color.r*255, color.g*255, color.b*255));
	pRender->draw(shape); 
}

#pragma warning(default:4244) //float to SF::Uint8