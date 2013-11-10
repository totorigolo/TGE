#pragma once
#include "LivingBeing.h"
#include "../Resources/Texture.h"

#include <SFML/Graphics.hpp>
#include <string>

class InputManager;
class Player : public LivingBeing
{
public:
	enum MovementEvent
	{
		Left,
		Right,
		Jump,
		Crounch
	};

public:
	// Ctor & dtor
	Player(b2Vec2 position, std::shared_ptr<Texture> texture, int layer = 1, unsigned int ID = 0U);
	virtual ~Player();

	// Mise à jour
	virtual void Update();

private:
	// Gestion des évènements
	void UpdateEvents();

private:
	// Gestionnaire d'évènements
	InputManager &mInputManager;
};