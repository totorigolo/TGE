#include "MainMenu.h"
#include "DevGame.h"
#include "../../Tools/Dialog.h"
#include <vector>

// Ctor & Dtor
MainMenu_State::MainMenu_State()
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

	// Change le titre de la fenêtre
	window->setTitle("Inverse Way Liberation - Menu principal");

	//return DevGame_State::GetPInstance();
	
	// Charge le fond
	sf::Texture fondTex;
	if (!fondTex.loadFromFile("tex/main_menu_bckg.jpg"))
		Dialog::Error("Impossible de charger l'image de fond du menu.");
	sf::Sprite fond(fondTex);

	// Charge la flèche
	sf::Texture arrowTex;
	if (!arrowTex.loadFromFile("tex/menu_arrow.png"))
		Dialog::Error("Impossible de charger l'image de flèche du menu.");
	sf::Sprite arrow(arrowTex);

	// Charge la police
	sf::Font font;
	if (!font.loadFromFile("tex/calibri.ttf"))
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
	int index = 0;

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
			
			// Flèche haut
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
			{
				--index;
				if (index < 0)
					index = entries.size() - 1;
			}
			// Flèche bas
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
			{
				++index;
				if (index >= static_cast<int>(entries.size()))
					index = 0;
			}

			// Entrée
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
			{
				switch (index)
				{
				case 0:
					return &DevGame_State::GetInstance();
					break;
				case 1:
					//return Editor_State::GetPInstance();
					break;
				case 2:
					// TODO: Système de menu plus poussé
					//return Configuration_State::GetPInstance();
					break;
				case 3:
				default:
					return Exit_State;
					break;
				}
			}
		}

		// Flèche
		arrow.setPosition(x - arrowTex.getSize().x - 10.f, y + index * stepY);

		// Affichage
		window->clear();
		
		window->draw(fond);
		window->draw(arrow);
		for (auto it = entries.begin(); it != entries.end(); ++it)
		{
			window->draw(*it);
		}

		window->display();
	}
	
	return Exit_State;
}