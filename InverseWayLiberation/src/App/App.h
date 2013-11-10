#pragma once
#include "../Tools/Singleton.h"

#include <SFML/Graphics.hpp>

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
};
