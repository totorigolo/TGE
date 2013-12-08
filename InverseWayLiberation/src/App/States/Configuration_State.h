#pragma once
#include "State.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

class Configuration_State : public State, public Singleton<Configuration_State>
{
protected:
	friend class Singleton<Configuration_State>;

	// Ctor & Dtor
	Configuration_State();
	virtual ~Configuration_State();

public:
	// Exécute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app);

	// Gestion de la GUI
	void CreateGUI();
	void EmptyGUI();

protected:
	// Callbacks
	void OnExit();

private:
	// Etat
	bool mQuit;

	// InputManager
	InputManager &mInputManager;

	/* GUI */
	// Fenêtre et Desktop
	sfg::SFGUI *mSfGUI;
	sfg::Desktop mDesktop;
	sf::Clock mGUIElapsedTime;

	// Eléments
	sfg::Window::Ptr mWindow;
	sfg::Box::Ptr mVBox;
	sfg::Label::Ptr mLabel;
	sfg::Button::Ptr mExit;
};