#include "Editor_State.h"
#include "MainMenu_State.h"
#include "../../Editor/Editor.h"

// Ctor & Dtor
Editor_State::Editor_State() {
}

Editor_State::~Editor_State() {
}

// Exécute le State
State *Editor_State::Run(App *app) {
    // Obtient un pointeur vers la fenêtre
    sf::RenderWindow *window = app->GetRenderWindow();
    window->setFramerateLimit(60U);

    // Change le titre de la fenêtre
    window->setTitle("totorigolo's Game Engine - Editeur");

    // Lance le jeu
    Editor *editor = new Editor(*window);
    try {
        editor->Run();
    } catch (std::exception const &e) {
        std::cerr << "L'éditeur a crashé :(" << std::endl
                  << e.what() << std::endl;
    }
    delete editor;

    return &MainMenu_State::GetInstance();
}
