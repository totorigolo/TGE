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

// Ex�cute le State actuel
bool StateSystem::Run(App *app)
{
	// Retient qu'on est en train de s'�x�cuter
	mIsRunning = true;

	// V�rifie que le state ne signale pas la fin de l'application
	if (mCurrentState == Exit_State)
	{
		mIsRunning = false;
		return false;
	}

	// Si on le doit, on ex�cuter le State
	if (mIsRunning)
		mCurrentState = mCurrentState->Run(app);

	// On signale qu'on en a pas fini
	return true;
}
