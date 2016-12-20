#pragma once
#include "State.h"

class Game_State : public State, public Singleton<Game_State>
{
protected:
	friend class Singleton<Game_State>;

	// Ctor & Dtor
	Game_State();
	virtual ~Game_State();

public:
	// Exécute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app);
};
