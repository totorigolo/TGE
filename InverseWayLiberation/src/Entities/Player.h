#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "LivingBeing.h"
#include "../Game/InputManager.h"

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
	Player(b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer = 1);
	virtual ~Player();

	// Mise � jour
	virtual void Update();

private:
	// Gestion des �v�nements
	void UpdateEvents();

private:
	// Gestionnaire d'�v�nements
	InputManager &mInputManager;
};