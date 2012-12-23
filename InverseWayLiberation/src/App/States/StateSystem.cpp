#include "StateSystem.h"

// Ctor & Dtor
StateSystem::StateSystem()
	: mCurrentState(nullptr), mIsRunning(false)
{
}
StateSystem::~StateSystem()
{
	mCurrentState = nullptr;
}

// Change le state actuel
void StateSystem::ChangeState(State *state)
{
	mCurrentState = state;
}

// Exécute le State actuel
bool StateSystem::Run(App *app)
{
	// Retient qu'on est en train de s'éxécuter
	mIsRunning = true;

	// Vérifie que le state ne signale pas la fin de l'application
	if (mCurrentState == Exit_State)
	{
		mIsRunning = false;
		return false;
	}

	// Si on le doit, on exécuter le State
	if (mIsRunning)
		mCurrentState = mCurrentState->Run(app);

	// On signale qu'on en a pas fini
	return true;
}
