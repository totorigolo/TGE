#include "Dialog.h"
#include "Parser.h"
#include "utils.h"

namespace Dialog
{
	void Dialog(const std::string &image, const std::string &title, const sf::String &message, bool quit, int output)
	{
		// Cr�ation de la fen�tre et des �v�nements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 150, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// On �vite les �v�nements qui ne nous sont pas destin�s
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
		while (window->pollEvent(event));

		// Chargement de l'image
		sf::Texture triangle;
		bool afficherImg = false;
		if (triangle.loadFromFile(image))
			afficherImg = true;

		// Cr�ation du texte
		sf::Font f;
		f.loadFromFile("calibri.ttf");
		sf::Text text(message, f);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);

		// On compte le nombre de retour � la ligne
		int nbRetours = 1;
		for (unsigned int i = 0; i < message.getSize(); ++i)
			if (message[i] == '\n')
				++nbRetours;

		// On place le texte en cons�quences
		text.move(140.f, 60.f / nbRetours);

		// Calcul de l'emplacement du bouton
		float x = text.getGlobalBounds().left + 20.f;
		float y = text.getGlobalBounds().top + text.getGlobalBounds().height + 20.f;
		// Cr�ation du bouton
		sf::RectangleShape button(sf::Vector2f(140.f, 30.f));
		button.setFillColor(sf::Color::Transparent);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(1.f);
		button.setPosition(x, y);
		sf::Text txtButton((quit) ? ("Quitter") : ("Valider"), f);
		txtButton.scale(0.6f, 0.6f);
		txtButton.setColor(sf::Color::Black);
		txtButton.setPosition(x + 5.f, y + 2.f);

		// Taille de la fen�tre par rapport � la taille du bouton
		window->setSize(sf::Vector2u(window->getSize().x,
									static_cast<unsigned int>(button.getGlobalBounds().top + button.getGlobalBounds().height + 20.f)));

		// Cr�ation du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 30.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appuie sur le bouton rouge ou sur �chap : on quitte
				if (event.type == sf::Event::Closed ||
					(event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Escape)))
					window->close();

				// On g�re le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fen�tre
					windowView.reset(sf::FloatRect(sf::Vector2f(0.f, 0.f), u2f(window->getSize())));
				}

				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
						backgroung.setPosition(button.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on v�rifie si on est sur un bouton et on quitte la cas �ch�ant
				else if (event.type == sf::Event::MouseButtonReleased)
				{
					if (button.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
						window->close();
				}
			}
			// On efface l'�cran
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

			// On affiche l'�cran
			window->display();
		}

		// Si l'erreur est importante : on quitte
		if (quit)
			exit(output);
	}

	void Error(const sf::String &message, bool quit, int output)
	{
		Dialog("res/erreur.png", "Erreur", message, quit, output);
	}

	void Information(const sf::String &message)
	{
		Dialog("res/information.png", "Info", message);
	}

	const sf::String TextInput(const std::string &title, const sf::String &message, const sf::String& _default)
	{
		// Cr�ation de la fen�tre et des �v�nements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 170, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// Cr�ation du texte
		sf::Font f;
		f.loadFromFile("calibri.ttf");
		sf::Text text(message, f);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);
		text.setPosition(10.f, 10.f);

		// Calcul de l'emplacement de la zone d'entr�e
		float y = 10.f + text.getGlobalBounds().height + 30.f;

		// Cr�ation de la zone d'entr�e
		sf::Vertex input[] =
		{
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black)
		};

		// Cr�ation du texte entr�
		sf::Text inputText;
		inputText.setFont(f);
		inputText.scale(0.8f, 0.8f);
		inputText.setColor(sf::Color::Black);
		inputText.setPosition(10.f + 5.f, y + 5.f);
		inputText.setString(_default);

		// Calcul de l'emplacement du bouton
		y += 40.f + 20.f;
		// Cr�ation du bouton
		sf::RectangleShape button(sf::Vector2f(140.f, 30.f));
		button.setFillColor(sf::Color::Transparent);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(1.f);
		button.setPosition(10.f, y);
		sf::Text txtButton("Valider", f);
		txtButton.scale(0.6f, 0.6f);
		txtButton.setColor(sf::Color::Black);
		txtButton.setPosition(10.f + 5.f, y + 2.f);
		
		// Taille de la fen�tre par rapport � la taille du bouton
		window->setSize(sf::Vector2u(window->getSize().x,
									static_cast<unsigned int>(button.getGlobalBounds().top + button.getGlobalBounds().height + 20.f)));

		// Cr�ation du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 30.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appuie sur le bouton rouge ou sur �chap : on retourne le texte par d�faut
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				{
					window->close();
					return std::string();
				}

				// Si on appuie sur  entr�e : on retourne le texte
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
				{
					window->close();
				}

				// On g�re le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fen�tre
					windowView.reset(sf::FloatRect(sf::Vector2f(0.f, 0.f), u2f(window->getSize())));
				}

				// Si on appuie sur Retour arri�re : on efface la derni�re lettre
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace)
				{
					sf::String str = inputText.getString();
					if (str.getSize() > 0)
						str.erase(str.getSize() - 1, 1);
					inputText.setString(str);
				}

				// Si du texte est tapp� : on l'ajoute � la zone d'�dition
				else if (event.type == sf::Event::TextEntered)
                {
					if (event.text.unicode >= 33 && event.text.unicode < 255 && event.text.unicode != 127 && event.text.unicode != 240) // tri
						inputText.setString(inputText.getString() + static_cast<char>(event.text.unicode));
                }

				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
						backgroung.setPosition(button.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on v�rifie si on est sur un bouton et on quitte la cas �ch�ant
				else if (event.type == sf::Event::MouseButtonReleased)
				{
					if (button.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
						window->close();
				}
			}
			// On efface l'�cran
			window->clear(sf::Color(240, 240, 240)); // Beige
			window->setView(windowView);

			// On affiche le texte
			window->draw(text);

			// On affiche la zone d'entr�e
			window->draw(input, 5, sf::LinesStrip);

			// On affiche le texte entr�
			window->draw(inputText);

			// On affiche le fond de survol du bouton
			if (overFlying)
				window->draw(backgroung);

			// On affiche le bouton
			window->draw(button);
			window->draw(txtButton);

			// On affiche l'�cran
			window->display();
		}

		if (inputText.getString().getSize() > 0)
		{
			return inputText.getString();
		}
		return _default;
	}

	int NumberInput(const std::string &title, const sf::String &message, int _default)
	{
		// Cr�ation de la fen�tre et des �v�nements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 150, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// Cr�ation du texte
		sf::Font f;
		f.loadFromFile("tex/calibri.ttf");
		sf::Text text(message, f);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);
		text.setPosition(10.f, 10.f);

		// Calcul de l'emplacement de la zone d'entr�e
		float y = 10.f + text.getGlobalBounds().height + 30.f;

		// Cr�ation de la zone d'entr�e
		sf::Vertex input[] =
		{
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f + 400.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y + 40.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(10.f, y), sf::Color::Black)
		};

		// Cr�ation du texte entr�
		int number = _default;
		sf::Text inputText;
		inputText.setFont(f);
		inputText.scale(0.8f, 0.8f);
		inputText.setColor(sf::Color::Black);
		inputText.setPosition(10.f + 5.f, y + 5.f);
		inputText.setString(Parser::int2string(number));

		// Calcul de l'emplacement du bouton
		y += 40.f + 20.f;
		// Cr�ation du bouton
		sf::RectangleShape button(sf::Vector2f(140.f, 30.f));
		button.setFillColor(sf::Color::Transparent);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(1.f);
		button.setPosition(10.f, y);
		sf::Text txtButton("Valider", f);
		txtButton.scale(0.6f, 0.6f);
		txtButton.setColor(sf::Color::Black);
		txtButton.setPosition(10.f + 5.f, y + 2.f);
		
		// Taille de la fen�tre par rapport � la taille du bouton
		window->setSize(sf::Vector2u(window->getSize().x,
									static_cast<unsigned int>(button.getGlobalBounds().top + button.getGlobalBounds().height + 20.f)));

		// Cr�ation du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 30.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appuie sur le bouton rouge ou sur �chap : on retourne le texte par d�faut
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				{
					window->close();
					return _default;
				}

				// Si on appuie sur entr�e : on retourne le texte
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
					window->close();

				// On g�re le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fen�tre
					windowView.reset(sf::FloatRect(sf::Vector2f(0.f, 0.f), u2f(window->getSize())));
				}

				// Si on appuie sur retour arri�re : on efface la derni�re lettre
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace)
				{
					number /= 10;
					inputText.setString(Parser::int2string(number));
				}

				// Si on appuie sur une touche : on ajoute � la zone d'entr�e
				else if (event.type == sf::Event::KeyPressed)
				{
					number = number * 10 + Parser::key2int(event.key.code);
					inputText.setString(Parser::int2string(number));
				}

				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
						backgroung.setPosition(button.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on v�rifie si on est sur un bouton et on quitte la cas �ch�ant
				else if (event.type == sf::Event::MouseButtonReleased)
				{
					if (button.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
						window->close();
				}
			}
			// On efface l'�cran
			window->clear(sf::Color(240, 240, 240)); // Beige
			window->setView(windowView);

			// On affiche le texte
			window->draw(text);

			// On affiche la zone d'entr�e
			window->draw(input, 5, sf::LinesStrip);

			// On affiche le texte entr�
			window->draw(inputText);

			// On affiche le fond de survol du bouton
			if (overFlying)
				window->draw(backgroung);

			// On affiche le bouton
			window->draw(button);
			window->draw(txtButton);

			// On affiche l'�cran
			window->display();
		}
		return number;
	}

	int ButtonChoice(const std::string &title, const sf::String &message, const sf::String &one, const sf::String &two, int _default)
	{
		// Cr�ation de la fen�tre et des �v�nements
		sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(500, 150, 32), title);
		window->setFramerateLimit(20);
		sf::Event event;
		sf::View windowView(window->getDefaultView());

		// Cr�ation du texte
		sf::Font f;
		f.loadFromFile("calibri.ttf");
		sf::Text text(message, f);
		text.scale(0.8f, 0.8f);
		text.setColor(sf::Color::Black);
		text.setPosition(10.f, 10.f);

		// Calcul de l'emplacement des boutons
		float y = ((10.f + text.getGlobalBounds().height + 30.f) + 150.f) / 2.f - (40.f / 2.f);
		float x1 = ((500.f / 2.f) / 2.f) - (140.f / 2.f);
		float x2 = (500.f * (3.f / 4.f)) - (140.f / 2.f);

		// Cr�ation du bouton 1
		sf::RectangleShape button1(sf::Vector2f(140.f, 40.f));
		button1.setFillColor(sf::Color::Transparent);
		button1.setOutlineColor(sf::Color::Black);
		button1.setOutlineThickness(2.f);
		button1.setPosition(x1, y);
		sf::Text txtButton1(one, f);
		txtButton1.scale(0.8f, 0.8f);
		txtButton1.setColor(sf::Color::Black);
		txtButton1.setPosition(x1 + 5.f, y + 5.f);

		// Cr�ation du bouton 2
		sf::RectangleShape button2(sf::Vector2f(140.f, 40.f));
		button2.setFillColor(sf::Color::Transparent);
		button2.setOutlineColor(sf::Color::Black);
		button2.setOutlineThickness(2.f);
		button2.setPosition(x2, y);
		sf::Text txtButton2(two, f);
		txtButton2.scale(0.8f, 0.8f);
		txtButton2.setColor(sf::Color::Black);
		txtButton2.setPosition(x2 + 5.f, y + 5.f);
		
		// Taille de la fen�tre par rapport � la taille du bouton
		window->setSize(sf::Vector2u(window->getSize().x,
									static_cast<unsigned int>(button1.getGlobalBounds().top + button1.getGlobalBounds().height + 20.f)));

		// Cr�ation du fond de survol
		sf::RectangleShape backgroung(sf::Vector2f(140.f, 40.f));
		backgroung.setFillColor(sf::Color(255, 0, 0, 128));

		// Boucle d'affichage
		int button = 0;
		bool overFlying = false;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				// Si on appui sur le bouton rouge ou sur �chap ou sur entr�e : on fait rien
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				{
					window->close();
					return _default;
				}

				// On g�re le redimentionnement
				else if (event.type == sf::Event::Resized)
				{
					// Vue de la fen�tre
					windowView.reset(sf::FloatRect(sf::Vector2f(0.f, 0.f), u2f(window->getSize())));
				}
				// Si la souris survole un bouton : on met le fond
				else if (event.type == sf::Event::MouseMoved)
				{
					overFlying = true;
					if (button1.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
						backgroung.setPosition(button1.getPosition());
					else if (button2.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
						backgroung.setPosition(button2.getPosition());
					else
						overFlying = false;
				}

				// Clic gauche : on v�rifie si on est sur un bouton et on retourne sa valeur
				else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::KeyReleased)
				{
					if (button1.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
						event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1)
						button = 1;
					else if (button2.getGlobalBounds().contains(i2f(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
						event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2)
						button = 2;

					if (button != 0)
						window->close();
				}
			}
			// On efface l'�cran
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

			// On affiche l'�cran
			window->display();
		}
		return button;
	}
}
