#include "InputHandler.h"
#include "GameGlobals.h"
#include <iostream>
#include "Camera.h"

sf::Vector2f InputHandler::LKPMouse = sf::Vector2f(0.f,0.f);
sf::RenderWindow *InputHandler::App = NULL;

bool InputHandler::IsKeyPressed(sf::Keyboard::Key Key)
{
	if (!gGlobals.InFocus)
		return false;
	return sf::Keyboard::isKeyPressed(Key);
}

bool InputHandler::IsMouseButtonPressed(sf::Mouse::Button Button)
{
	if (!gGlobals.InFocus)
		return false;
	return sf::Mouse::isButtonPressed(Button);
}

Vector2 InputHandler::GetMousePosWorld()
{
	Vector2 WindPos = GetMousePos();
	Vector2 ViewCentre(gGlobals.GameWidth / 2.f, gGlobals.GameHeight / 2.f);

	WindPos = WindPos + sCamera::GetCentre() - ViewCentre;

	return WindPos;
}

Vector2 InputHandler::GetMousePos()
{
	if (!gGlobals.InFocus)
		return LKPMouse;
	sf::Vector2f Pos;
	sf::Vector2i iPos = sf::Mouse::getPosition(*App);
	Pos.x = float(iPos.x);
	Pos.y = float(iPos.y);
	Pos = App->convertCoords(sf::Vector2i(Pos.x, Pos.y));
	LKPMouse = Pos;

	return Vector2(Pos);
}
