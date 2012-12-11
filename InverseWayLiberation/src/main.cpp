#include <iostream>
#include <SFML\Graphics.hpp>
#include <Thor\Events.hpp>
#include "Box2DGame.h"

// Mettre la fonction main() en tant que fonction statique d'une classe

int main()
{
	// Crée la fenêtre de rendu
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Inverse Way Liberation");
	//window.setFramerateLimit(60U);
	window.setVerticalSyncEnabled(true);

	// Crée le jeu
	Box2DGame jeu(window);
	jeu.Run();

	// Action de fermeture de la fenêtre + callback
	thor::ActionMap<std::string> map(window);
	thor::ActionMap<std::string>::CallbackSystem system;
	map["closed"] = thor::Action(sf::Keyboard::Escape, thor::Action::ReleaseOnce) || thor::Action(sf::Event::Closed);
	system.connect("closed", std::bind(&sf::RenderWindow::close, &window));

	while (window.isOpen())
	{
		map.update();
		map.invokeCallbacks(system);
		window.clear(sf::Color::Black);
		window.display();
	}

	return 0;
}
