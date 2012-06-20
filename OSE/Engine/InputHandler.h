#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Utilities/Vector2.h"

class InputHandler
{
private:
	static sf::Vector2f LKPMouse;
	static sf::RenderWindow *App;
public:
	static void SetWindow(sf::RenderWindow *wind) {App = wind;};
	static bool IsKeyPressed(sf::Keyboard::Key Key);
	static bool IsMouseButtonPressed(sf::Mouse::Button Button);
	static Vector2 GetMousePos();
	static Vector2 GetMousePosWorld();
};
