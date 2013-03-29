#pragma once
#include "../../Tools/Singleton.h"

class App;
class State;
class StateSystem : public Singleton<StateSystem>
{
protected:
	friend class Singleton<StateSystem>;

	// Ctor & Dtor
	StateSystem();
	virtual ~StateSystem();

public:
	// Change le state actuel
	void ChangeState(State *state);

	// Exécute le State actuel, et retourne en fonction de si il a finit.
	// Le State actuel passe au suivant à la fin de la fonction.
	bool Run(App *app);

private:
	// Etat
	bool mIsRunning;

	// State actuel
	State *mCurrentState;
};
