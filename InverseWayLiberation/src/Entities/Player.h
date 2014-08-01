#pragma once
#include "Entity.h"
#include "Hum.h"
#include "App/SpyedKey.h"
#include "../Resources/Texture.h"

class InputManager;
class Player : public Hum
{
public:
	// Ctor & dtor
	Player();
	virtual ~Player();

	// Mise � jour
	virtual void Update();

protected:
	// Gestion des �v�nements
	void UpdateEvents();

private:
	// Managers
	InputManager &mInputManager;

	// Touches surveill�es
	SpyedKey mJumpKey;
};