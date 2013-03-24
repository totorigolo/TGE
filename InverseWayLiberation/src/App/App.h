#pragma once
#include "../Tools/Singleton.h"
#include <SFML/Graphics.hpp>

class StateSystem;
class App : public Singleton<App>
{
public:
	// Ctor & Dtor
	App();
	virtual ~App();
	
	// Exécute l'application
	int Execute();

	// Accesseurs
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
};
