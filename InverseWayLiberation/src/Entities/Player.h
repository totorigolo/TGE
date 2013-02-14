#pragma once
#include <SFML/Graphics.hpp>
#include "LivingBeing.h"
#include <string>

enum class PlayerEvent
{
	Left,
	Right,
	Jump,
	Crounch
};

class Player : public LivingBeing
{
public:
	// Ctor & dtor
	Player(World *world, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer = 1);
	virtual ~Player();

	// Mise � jour
	//virtual void Update();

	// Gestion des �v�nements
	void GetEvent(const PlayerEvent &playerEvent);

private:
};