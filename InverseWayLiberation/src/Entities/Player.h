#pragma once
#include <SFML/Graphics.hpp>
#include "LivingBeing.h"
#include <string>

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
	Player(PhysicManager *physicMgr, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer = 1);
	virtual ~Player();

	// Mise à jour
	virtual void Update();

	// Gestion des évènements
	void GetEvent(const MovementEvent &playerEvent);

private:
};