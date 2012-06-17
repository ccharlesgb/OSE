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
   sf::Shape Shape; 
   Shape.SetOutlineThickness(1.f); 
   for (int32 i = 0; i < vertexCount; ++i) 
   { 
	  Vector2 pos = RENDERER->GameToSFML(Vector2(vertices[i]) * PIXELS_PER_METRE);
      Shape.AddPoint(pos.x, pos.y, sf::Color(128*color.r, 128*color.g, 128*color.b, 128), sf::Color(color.r*255, color.g*255, color.b*255, 128)); 
   } 
	pRender->Draw(Shape); 
} 
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
{ 
   sf::Shape Shape;
   Shape.SetOutlineThickness(1.f);
   for (int32 i = 0; i < vertexCount; ++i)
   {
	  Vector2 pos = RENDERER->GameToSFML(Vector2(vertices[i]) * PIXELS_PER_METRE);
      Shape.AddPoint(pos.x, pos.y, sf::Color(128*color.r, 128*color.g, 128*color.b, 128), sf::Color(color.r*255, color.g*255, color.b*255)); 
   } 
   pRender->Draw(Shape); 
} 
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) 
{ 
	Vector2 pos = RENDERER->GameToSFML(Vector2(center) * PIXELS_PER_METRE);
   sf::Shape Shape = sf::Shape::Circle(pos.x, pos.y, radius * PIXELS_PER_METRE, sf::Color(128*color.r, 128*color.g, 128*color.b, 128), 1.f, sf::Color(color.r*255, color.g*255, color.b*255)); 
   pRender->Draw(Shape); 
} 
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) 
{ 
	Vector2 pos = RENDERER->GameToSFML(Vector2(center) * PIXELS_PER_METRE);
   sf::Shape Shape = sf::Shape::Circle(0.f, 0.f, radius * PIXELS_PER_METRE, sf::Color(128*color.r, 128*color.g, 128*color.b, 128), 1.f, sf::Color(color.r*255, color.g*255, color.b*255)); 
   Shape.SetPosition(pos.x, pos.y); 
   pRender->Draw(Shape); 
} 
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) 
{ 
	Vector2 pos1 = RENDERER->GameToSFML(p1);
	Vector2 pos2 = RENDERER->GameToSFML(p2);
   sf::Shape Line = sf::Shape::Line(pos1.x, pos1.y, pos2.x, pos2.y, 1.f, sf::Color(color.r*255, color.g*255, color.b*255)); 
   pRender->Draw(Line); 
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
	pRender->Draw(sf::Shape::Circle(pos.x,pos.y, size, sf::Color(color.r*255, color.g*255, color.b*255))); 
} 
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color) 
{ 
	Vector2 lower = RENDERER->GameToSFML(aabb->lowerBound);
	Vector2 upper = RENDERER->GameToSFML(aabb->upperBound);
	pRender->Draw(sf::Shape::Rectangle(lower.x, lower.y, (lower.x+upper.x), (lower.y+upper.y), sf::Color(color.r*255, color.g*255, color.b*255))); 
}

#pragma warning(default:4244) //float to SF::Uint8