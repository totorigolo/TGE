#pragma once
#include "../../Tools/Singleton.h"

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

	// Ex�cute le State actuel, et retourne en fonction de si il a finit.
	// Le State actuel passe au suivant � la fin de la fonction.
	bool Run(App *app);

private:
	// Etat
	bool mIsRunning;

	// State actuel
	State *mCurrentState;
};
