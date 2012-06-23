#include "InputHandler.h"
#include "GameGlobals.h"
#include "Camera.h"

Vector2 InputHandler::LKPMouse = Vector2(0.f,0.f);
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
	Vector2 ViewCentre(gGlobals.GameWidth / 2.f, -gGlobals.GameHeight / 2.f);
	WindPos.y *= -1;
	WindPos = (WindPos - ViewCentre) * sCamera::GetZoom();
	WindPos = WindPos + sCamera::GetCentre();
	return WindPos;
}

Vector2 InputHandler::GetMousePos()
{
	if (!gGlobals.InFocus)
		return LKPMouse;
	sf::Vector2f Pos;
	sf::Vector2i iPos = sf::Mouse::getPosition(*App);

	Pos = App->convertCoords(iPos);
	LKPMouse = Vector2(Pos.x, Pos.y);

	return Vector2(Pos.x, Pos.y);
}
