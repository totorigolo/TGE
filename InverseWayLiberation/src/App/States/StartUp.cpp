#include "StartUp.h"
#include "MainMenu.h"
#include <vector>

// Ctor & Dtor
StartUp_State::StartUp_State()
{
}
StartUp_State::~StartUp_State()
{
}

// Exécute le State
State* StartUp_State::Run(App *app)
{
	// Récupère la fenêtre
	sf::RenderWindow *window = app->GetRenderWindow();

	// Change le titre de la fenêtre
	window->setTitle("Inverse Way Liberation - Splash");

	//return MainMenu_State::GetPInstance();

	// Charge les textures
	std::vector<sf::Texture*> textures;
	sf::Texture t1;
	if (!t1.loadFromFile("tex/splash_libs.jpg"))
		return MainMenu_State::GetPInstance();
	textures.push_back(&t1);
	sf::Texture t2;
	if (!t2.loadFromFile("tex/splash_totorigolo.jpg"))
		return MainMenu_State::GetPInstance();
	textures.push_back(&t2);

	// Crée le sprite
	sf::Sprite sprite;

	// Variables pour l'effet
	int state = 0;
	int splash = 0;
	sf::Clock clock;
	float opacity = 0.f;

	// Boucle d'affichage
	bool quit = false;
	while (window->isOpen() && !quit)
	{
		// Evènements
		sf::Event event;
		while (window->pollEvent(event))
		{
			// Clic sur la petite croix rouge
			if (event.type == sf::Event::Closed || 
				(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
				quit = true;

			// N'importe quelle autre touche : splash suivant
			else if (event.type == sf::Event::KeyReleased)
			{
				opacity = 0.f;
				state = 0;
			}
		}

		// Effet
		if (state == 0) // Changement de splash
		{
			if (splash < static_cast<int>(textures.size()))
			{
				sprite.setTexture(*textures.at(splash++));
				state = 1;
			}
			else
				state = 4;
		}
		else if (state == 1) // Apparition
		{
			opacity += clock.getElapsedTime().asSeconds() / 3.f;
			clock.restart();

			if (opacity >= 1.f)
			{
				state = 2;
				opacity = 1.f;
			}
		}
		else if (state == 2) // Attente
		{
			if (clock.getElapsedTime().asSeconds() >= 2.f)
			{
				state = 3;
				clock.restart();
			}
		}
		else if (state == 3) // Disparition
		{
			opacity -= clock.getElapsedTime().asSeconds() / 1.6f;
			clock.restart();

			if (opacity <= 0.f)
			{
				state = 0;
				opacity = 0.f;
			}
		}
		else if (state == 4) // Fin
		{
			quit = true;
		}
		// Change l'opacité
		sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(opacity * 255)));

		// Affichage
		window->clear();

		window->draw(sprite);

		window->display();
	}

	// Passe au menu principal
	return MainMenu_State::GetPInstance();
}