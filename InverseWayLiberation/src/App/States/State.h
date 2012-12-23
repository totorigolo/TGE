#pragma once
#include "../../Tools/Singleton.h"
#include "../App.h"

// State prédéfini.
// Il signale que le programme doit quitter.
#define Exit_State nullptr

class App;
class State
{
public:
	// Ctor & Dtor
	State()
	{
	}
	virtual ~State()
	{
	}
	
	// Exécute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app) = 0;
};
