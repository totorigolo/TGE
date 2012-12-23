#pragma once
#include "State.h"

class DevGame_State : public State, public Singleton<DevGame_State>
{
public:
	// Ctor & Dtor
	DevGame_State();
	virtual ~DevGame_State();

	// Exécute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app);
};
