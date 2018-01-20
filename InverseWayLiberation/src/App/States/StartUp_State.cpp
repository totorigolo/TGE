#include "StartUp_State.h"
#include "MainMenu_State.h"
#include "../SpyedKey.h"

// Ctor & Dtor
StartUp_State::StartUp_State()
	: mInputManager(InputManager::GetInstance())
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
	window->setFramerateLimit(30U);
	window->setView(window->getDefaultView());

	// Evènements
	SpyedKey key_return(sf::Keyboard::Return);

	// Change le titre de la fenêtre
	window->setTitle("totorigolo's Game Engine - Splash");

	// Charge les textures
	std::vector<sf::Texture*> textures;
	sf::Texture t1;
	if (!t1.loadFromFile("tex/splash_libs.jpg"))
		return &MainMenu_State::GetInstance();
	textures.push_back(&t1);
	sf::Texture t2;
	if (!t2.loadFromFile("tex/splash_totorigolo.jpg"))
		return &MainMenu_State::GetInstance();
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
		mInputManager.Update();
		if (mInputManager.HasQuitted())
		{
			quit = true;
		}
		if (mInputManager.KeyReleased(sf::Keyboard::Return))
		{
			opacity = 0.f;
			state = 0;
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

		// Applique l'opacité
		sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(opacity * 255)));

		// Affichage
		window->clear();
		window->draw(sprite);
		window->display();
	}

	// Passe au menu principal
	return &MainMenu_State::GetInstance();
}