#pragma once
#include "State.h"

class MainMenu_State : public State, public Singleton<MainMenu_State>
{
public:
	// Ctor & Dtor
	MainMenu_State();
	virtual ~MainMenu_State();

	// Exécute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app);
};