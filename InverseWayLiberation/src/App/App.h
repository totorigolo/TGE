#pragma once
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
	// Ex�cute l'application
	int Execute();

	// Accesseurs
	sfg::SFGUI* GetSfGUI();
	sf::RenderWindow* GetRenderWindow();

protected:
	// Appel�s par Execute();
	bool Init();
	void CleanUp();

private:
	// Syst�me de States
	StateSystem &mStateSystem;

	// Fen�tre de rendu
	sf::RenderWindow *mRenderWindow;

	// InputManager
	InputManager &mInputManager;

	// GUI
	sfg::SFGUI *mSfGUI;
};
