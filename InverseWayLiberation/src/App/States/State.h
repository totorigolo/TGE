#pragma once
#include "../App.h"

// State pr�d�fini.
// Il signale que le programme doit quitter.
#define Exit_State nullptr

class App;
class State : public NonCopyable
{
public:
	// Ctor & Dtor
	State()
	{
	}
	virtual ~State()
	{
	}
	
	// Ex�cute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app) = 0;
};
