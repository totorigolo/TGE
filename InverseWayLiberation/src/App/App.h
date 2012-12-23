#pragma once
#include "../Tools/Singleton.h"
#include "../App/States/StateSystem.h"

#include <SFML/Graphics.hpp>

class StateSystem;
class App : public Singleton<App>
{
public:
	// Ctor & Dtor
	App();
	virtual ~App();
	
	// Ex�cute l'application
	int Execute();

	// Accesseurs
	sf::RenderWindow* GetRenderWindow();

protected:
	// Appel�s par Execute();
	bool Init();
	void CleanUp();

private:
	StateSystem &mStateSystem;

	sf::RenderWindow *mRenderWindow;
};
