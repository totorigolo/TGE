#pragma once
#include "State.h"

class StartUp_State : public State, public Singleton<StartUp_State>
{
public:
	// Ctor & Dtor
	StartUp_State();
	virtual ~StartUp_State();

	// Exécute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app);
};