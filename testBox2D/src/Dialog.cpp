#include "Dialog.h"
#include "Parser.h"

namespace Dialog
{
	// TODO: Retours à la ligne automatiques

	void Dialog(const std::string &image, const std::string &title, const std::string &message, bool quit, int output)
	{
		// Création de la fenêtre et des évènements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 150, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// On évite les évènements qui ne nous sont pas destinés
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
		while (window->pollEvent(event));

		// Chargement de l'image
		sf::Texture triangle;
		bool afficherImg = false;
		if (triangle.loadFromFile(image))
			afficherImg = true;

		// Création du texte
		sf::Text text(message);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);

		// On compte le nombre de retour à la ligne
		int nbRetours = 1;
		for (unsigned int i = 0; i < message.size(); ++i)
			if (message.at(i) == '\n')
				++nbRetours;

		// On place le texte en conséquences
		text.move(140.f, 60.f / nbRetours);
		
		// Calcul de l'emplacement du bouton
		float x = text.getGlobalBounds().left + 20.f;
		float y = text.getGlobalBounds().top + text.getGlobalBounds().height + 20.f;
		// Création du bouton
		sf::RectangleShape button(sf::Vector2f(140.f, 30.f));
		button.setFillColor(sf::Color::Transparent);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(1.f);
		button.setPosition(x, y);
		sf::Text txtButton((quit) ? ("Quitter") : ("Valider"));
		txtButton.scale(0.6f, 0.6f);
		txtButton.setColor(sf::Color::Black);
		txtButton.setPosition(x + 5.f, y + 2.f);
		
		// Création du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 30.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appuie sur le bouton rouge ou sur échap : on quitte
				if (event.type == sf::Event::Closed ||
					event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Escape))
					window->close();

				// On gère le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fenêtre
					windowView.reset(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
				}

				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
						backgroung.setPosition(button.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on vérifie si on est sur un bouton et on retourne sa valeur
				else if (event.type == sf::Event::MouseButtonReleased)
				{
					if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						window->close();
				}
			}
			// On efface l'écran
			window->clear(sf::Color(240, 240, 240)); // Beige
			window->setView(windowView);

			// On dessine le triangle
			if (afficherImg)
				window->draw(sf::Sprite(triangle));

			// On affiche le texte
			window->draw(text);
			
			// On affiche le fond de survol du bouton
			if (overFlying)
				window->draw(backgroung);

			// On affiche le bouton
			window->draw(button);
			window->draw(txtButton);

			// On affiche l'écran
			window->display();
		}

		// Si l'erreur est importante : on quitte
		if (quit)
			exit(output);
	}

	void Error(const std::string &message, bool quit, int output)
	{
		Dialog("res/erreur.png", "Erreur", message, quit, output);
	}

	void Information(const std::string &message)
	{
		Dialog("res/information.png", "Info", message);
	}
	
	const std::string TextInput(const std::string &title, const std::string &message, const std::string& default)
	{
		// Création de la fenêtre et des évènements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 170, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// Création du texte
		sf::Text text(message);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);
		text.setPosition(10.f, 10.f);
		
		// Calcul de l'emplacement de la zone d'entrée
		float y = 10.f + text.getGlobalBounds().height + 30.f;

		// Création de la zone d'entrée
		sf::Vertex input[] =
		{
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black)
		};

		// Création du texte entré
		sf::Text inputText;
		inputText.scale(0.8f, 0.8f);
		inputText.setColor(sf::Color::Black);
		inputText.setPosition(10.f + 5.f, y + 5.f);

		// Calcul de l'emplacement du bouton
		y += 40.f + 20.f;
		// Création du bouton
		sf::RectangleShape button(sf::Vector2f(140.f, 30.f));
		button.setFillColor(sf::Color::Transparent);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(1.f);
		button.setPosition(10.f, y);
		sf::Text txtButton("Valider");
		txtButton.scale(0.6f, 0.6f);
		txtButton.setColor(sf::Color::Black);
		txtButton.setPosition(10.f + 5.f, y + 2.f);
		
		// Création du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 30.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appuie sur le bouton rouge ou sur échap : on retourne le texte par défaut
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				{
					window->close();
					return default;
				}

				// Si on appuie sur  entrée : on retourne le texte
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
					window->close();
				
				// On gère le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fenêtre
					windowView.reset(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
				}

				// Si on appuie sur retour arrière : on efface la dernière lettre
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Back)
				{
					std::string str = inputText.getString();
					if (str.size() > 0)
						str.erase(str.size() - 1, 1);
					inputText.setString(str);
				}
				
				/*/ Si on appuie sur une touche : on ajoute à la zone d'entrée
				else if (event.type == sf::Event::KeyPressed)
					inputText.setString(inputText.getString() + key2string(event.key.code, event.key.shift));*/

				// Si du texte est tappé : on l'ajoute à la zone d'édition
				else if (event.type == sf::Event::TextEntered)
					if (event.text.unicode >= 33 && event.text.unicode < 255 && event.text.unicode != 127 && event.text.unicode != 240) // tri
						inputText.setString(inputText.getString() + static_cast<char>(event.text.unicode));


				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
						backgroung.setPosition(button.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on vérifie si on est sur un bouton et on retourne sa valeur
				else if (event.type == sf::Event::MouseButtonReleased)
				{
					if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						window->close();
				}
			}
			// On efface l'écran
			window->clear(sf::Color(240, 240, 240)); // Beige
			window->setView(windowView);

			// On affiche le texte
			window->draw(text);

			// On affiche la zone d'entrée
			window->draw(input, 5, sf::LinesStrip);

			// On affiche le texte entré
			window->draw(inputText);
			
			// On affiche le fond de survol du bouton
			if (overFlying)
				window->draw(backgroung);

			// On affiche le bouton
			window->draw(button);
			window->draw(txtButton);

			// On affiche l'écran
			window->display();
		}
		return inputText.getString();
	}

	int NumberInput(const std::string &title, const std::string &message, int default)
	{
		// Création de la fenêtre et des évènements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 150, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// Création du texte
		sf::Text text(message);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);
		text.setPosition(10.f, 10.f);

		// Calcul de l'emplacement de la zone d'entrée
		float y = 10.f + text.getGlobalBounds().height + 30.f;

		// Création de la zone d'entrée
		sf::Vertex input[] =
		{
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black)
		};

		// Création du texte entré
		int number = 0;
		sf::Text inputText;
		inputText.scale(0.8f, 0.8f);
		inputText.setColor(sf::Color::Black);
		inputText.setPosition(10.f + 5.f, y + 5.f);
		
		// Calcul de l'emplacement du bouton
		y += 40.f + 20.f;
		// Création du bouton
		sf::RectangleShape button(sf::Vector2f(140.f, 30.f));
		button.setFillColor(sf::Color::Transparent);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(1.f);
		button.setPosition(10.f, y);
		sf::Text txtButton("Valider");
		txtButton.scale(0.6f, 0.6f);
		txtButton.setColor(sf::Color::Black);
		txtButton.setPosition(10.f + 5.f, y + 2.f);
		
		// Création du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 30.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appuie sur le bouton rouge ou sur échap : on retourne le texte par défaut
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				{
					window->close();
					return default;
				}
				
				// Si on appuie sur entrée : on retourne le texte
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
					window->close();
				
				// On gère le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fenêtre
					windowView.reset(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
				}

				// Si on appuie sur retour arrière : on efface la dernière lettre
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Back)
				{
					number /= 10;
					inputText.setString(int2string(number));
				}

				// Si on appuie sur une touche : on ajoute à la zone d'entrée
				else if (event.type == sf::Event::KeyPressed)
				{
					number = number * 10 + key2int(event.key.code);
					inputText.setString(int2string(number));
				}

				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
						backgroung.setPosition(button.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on vérifie si on est sur un bouton et on retourne sa valeur
				else if (event.type == sf::Event::MouseButtonReleased)
				{
					if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						window->close();
				}
			}
			// On efface l'écran
			window->clear(sf::Color(240, 240, 240)); // Beige
			window->setView(windowView);

			// On affiche le texte
			window->draw(text);

			// On affiche la zone d'entrée
			window->draw(input, 5, sf::LinesStrip);

			// On affiche le texte entré
			window->draw(inputText);
			
			// On affiche le fond de survol du bouton
			if (overFlying)
				window->draw(backgroung);

			// On affiche le bouton
			window->draw(button);
			window->draw(txtButton);

			// On affiche l'écran
			window->display();
		}
		return number;
	}

	int ButtonChoice(const std::string &title, const std::string &message, const std::string &one, const std::string &two, int default)
	{
		// Création de la fenêtre et des évènements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 150, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// Création du texte
		sf::Text text(message);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);
		text.setPosition(10.f, 10.f);

		// Calcul de l'emplacement des boutons
		float y = ((10.f + text.getGlobalBounds().height + 30.f) + 150.f) / 2.f - (40.f / 2.f);
		float x1 = ((500.f / 2.f) / 2.f) - (140.f / 2.f);
		float x2 = (500.f * (3.f / 4.f)) - (140.f / 2.f);
		
		// Création du bouton 1
		sf::RectangleShape button1(sf::Vector2f(140.f, 40.f));
		button1.setFillColor(sf::Color::Transparent);
		button1.setOutlineColor(sf::Color::Black);
		button1.setOutlineThickness(2.f);
		button1.setPosition(x1, y);
		sf::Text txtButton1(one);
		txtButton1.scale(0.8f, 0.8f);
		txtButton1.setColor(sf::Color::Black);
		txtButton1.setPosition(x1 + 5.f, y + 5.f);

		// Création du bouton 2
		sf::RectangleShape button2(sf::Vector2f(140.f, 40.f));
		button2.setFillColor(sf::Color::Transparent);
		button2.setOutlineColor(sf::Color::Black);
		button2.setOutlineThickness(2.f);
		button2.setPosition(x2, y);
		sf::Text txtButton2(two);
		txtButton2.scale(0.8f, 0.8f);
		txtButton2.setColor(sf::Color::Black);
		txtButton2.setPosition(x2 + 5.f, y + 5.f);

		// Création du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 40.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		int button = 0;
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appui sur le bouton rouge ou sur échap ou sur entrée : on fait rien
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				{
					window->close();
					return default;
				}
				
				// On gère le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fenêtre
					windowView.reset(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
				}
				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button1.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
						backgroung.setPosition(button1.getPosition());
					else if (button2.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
						backgroung.setPosition(button2.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on vérifie si on est sur un bouton et on retourne sa valeur
				else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::KeyReleased)
				{
					if (button1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) ||
						event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1)
						button = 1;
					else if (button2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) ||
						event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2)
						button = 2;

					if (button != 0)
						window->close();
				}
			}
			// On efface l'écran
			window->clear(sf::Color(240, 240, 240)); // Beige
			window->setView(windowView);

			// On affiche le texte
			window->draw(text);
			
			// On affiche le fond de survol
			if (overFlying)
				window->draw(backgroung);

			// On affiche le bouton 1
			window->draw(button1);
			window->draw(txtButton1);

			// On affiche le bouton 2
			window->draw(button2);
			window->draw(txtButton2);

			// On affiche l'écran
			window->display();
		}
		return button;
	}
}