#pragma once
#include <SFGUI/SFGUI.hpp>
#include "InputManager.h"

class StateSystem;
class App : public Singleton<App>
{
protected:
	friend class Singleton<App>;

	// Ctor & Dtor
	App();
	virtual ~App();
	
public:
	// Exécute l'application
	int Execute();

	// Accesseurs
	sfg::SFGUI* GetSfGUI();
	sf::RenderWindow* GetRenderWindow();

protected:
	// Appelés par Execute();
	bool Init();
	void CleanUp();

private:
	// Système de States
	StateSystem &mStateSystem;

	// Fenêtre de rendu
	sf::RenderWindow *mRenderWindow;

	// InputManager
	InputManager &mInputManager;

	// GUI
	sfg::SFGUI *mSfGUI;
};
