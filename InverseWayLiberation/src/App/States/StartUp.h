#pragma once
#include "State.h"

class StartUp_State : public State, public Singleton<StartUp_State>
{
protected:
	friend class Singleton<StartUp_State>;

	// Ctor & Dtor
	StartUp_State();
	virtual ~StartUp_State();

public:
	// Ex�cute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app);
};