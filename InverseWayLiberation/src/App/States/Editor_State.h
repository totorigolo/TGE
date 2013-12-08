#pragma once
#include "State.h"

class Editor_State : public State, public Singleton<Editor_State>
{
protected:
	friend class Singleton<Editor_State>;

	// Ctor & Dtor
	Editor_State();
	virtual ~Editor_State();

public:
	// Exécute le State
	// Retourne un pointeur sur le prochain State
	virtual State* Run(App *app);
};
