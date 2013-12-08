#include "MainMenu_State.h"
#include "Game_State.h"
#include "Editor_State.h"
#include "Configuration_State.h"
#include "../SpyedKey.h"
#include "../../Tools/Dialog.h"

#include <vector>

// Ctor & Dtor
MainMenu_State::MainMenu_State()
	: mInputManager(InputManager::GetInstance())
{
}
MainMenu_State::~MainMenu_State()
{
}

// Exécute le State
State* MainMenu_State::Run(App *app)
{
	// Récupère la fenêtre
	sf::RenderWindow *window = app->GetRenderWindow();
	window->setFramerateLimit(30U);
	window->setView(window->getDefaultView());

	// Evènements
	SpyedKey key_return(sf::Keyboard::Return);
	SpyedKey key_up(sf::Keyboard::Up);
	SpyedKey key_down(sf::Keyboard::Down);

	// Change le titre de la fenêtre
	window->setTitle("Inverse Way Liberation - Menu principal");

	//return DevGame_State::GetPInstance();
	
	// Charge le fond
	sf::Texture fondTex;
	if (!fondTex.loadFromFile("tex/main_menu_bckg.jpg"))
		Dialog::Error("Impossible de charger l'image de fond du menu.");
	sf::Sprite fond(fondTex);
	fond.setPosition(window->getSize().x / 2.f - fondTex.getSize().x / 2.f , 0.f);

	// Charge la flèche
	sf::Texture arrowTex;
	if (!arrowTex.loadFromFile("tex/menu_arrow.png"))
		Dialog::Error("Impossible de charger l'image de flèche du menu.");
	sf::Sprite arrow(arrowTex);

	// Charge la police
	sf::Font font;
	if (!font.loadFromFile("data/calibri.ttf"))
		Dialog::Error("Impossible de charger la police du menu.", true);

	// Crée les entrées du menu
	std::vector<sf::Text> entries;
	entries.push_back(sf::Text("Jouer", font));
	entries.push_back(sf::Text("Editeur", font));
	entries.push_back(sf::Text("Configuration", font));
	entries.push_back(sf::Text("Quitter", font));

	// Positionne les entrées
	float x = 320.f;
	float y = 250.f;
	float stepY = entries.front().getCharacterSize() + 10.f;
	for (unsigned int i = 0; i < entries.size(); ++i)
	{
		entries[i].setPosition(x, y + i * stepY);
	}

	// Variables pour le menu
	int index = 1;

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
		// Redimensionnement
		if (mInputManager.HasResized())
		{

		}
		// Flèche haut
		if (mInputManager.KeyReleased(sf::Keyboard::Up))
		{
			--index;
			if (index < 0)
				index = entries.size() - 1;
		}
		// Flèche bas
		if (mInputManager.KeyReleased(sf::Keyboard::Down))
		{
			++index;
			if (index >= static_cast<int>(entries.size()))
				index = 0;
		}
		// Entrée
		if (mInputManager.KeyReleased(sf::Keyboard::Return))
		{
			switch (index)
			{
			case 0:
				return &Game_State::GetInstance();
				break;
			case 1:
				return &Editor_State::GetInstance();
				break;
			case 2:
				return &Configuration_State::GetInstance();
				break;
			case 3:
			default:
				return Exit_State;
				break;
			}
		}

		// Flèche
		arrow.setPosition(x - arrowTex.getSize().x - 10.f, y + index * stepY);

		/* Affichage */
		window->clear();
		
		// Affiche le fond, la flèche
		window->draw(fond);
		window->draw(arrow);

		// Affiche les entrées du menu
		for (auto it = entries.begin(); it != entries.end(); ++it)
		{
			window->draw(*it);
		}

		// Finalise le rendu
		window->display();
	}

	return Exit_State;
}