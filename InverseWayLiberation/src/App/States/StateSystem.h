#pragma once
#include "../../Tools/Singleton.h"
#include "State.h"
#include "../App.h"

class App;
class State;
class StateSystem : public Singleton<StateSystem>
{
public:
	// Ctor & Dtor
	StateSystem();
	virtual ~StateSystem();

	// Change le state actuel
	void ChangeState(State *state);

	// Exécute le State actuel, et retourne en fonction de si il a finit.
	// Le State actuel passe au suivant à la fin de la fonction.
	bool Run(App *app);

private:
	bool mIsRunning;
	State *mCurrentState;
};
