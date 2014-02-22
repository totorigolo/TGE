#include "stdafx.h"
#include "Editor_State.h"
#include "MainMenu_State.h"
#include "../../Editor/Editor.h"

// Ctor & Dtor
Editor_State::Editor_State()
{
}
Editor_State::~Editor_State()
{
}

// Ex�cute le State
State* Editor_State::Run(App *app)
{
	// Obtient un pointeur vers la fen�tre
	sf::RenderWindow *window = app->GetRenderWindow();
	window->setFramerateLimit(60U);

	// Change le titre de la fen�tre
	window->setTitle("Inverse Way Liberation - DevGame");

	// Lance le jeu
	Editor *editor = new Editor(*window);
	editor->Run();
	delete editor;

	return &MainMenu_State::GetInstance();
}
