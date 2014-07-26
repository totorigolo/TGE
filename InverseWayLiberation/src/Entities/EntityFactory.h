#pragma once
#include "PolyChain.h"
#include "../Tools/vector_wrappers.h"

namespace EntityFactory
{
	// Charge une texture
	void LoadTexture(const std::string &name, const std::string &path);

	// Cr�e une d�co
	void CreateDeco(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Cr�e un BasicBody : Box
	void CreateBox(const b2Vec3 &posRot, b2BodyType type, const std::string &texture, int layer = 1);
	
	// Cr�e un BasicBody : Circle
	void CreateCircle(const b2Vec3 &posRot, b2BodyType type, const std::string &texture, int layer = 1);

	// Cr�e un PolyBody
	void CreatePolyBody(const vector_b2Vec2 &vectices, b2BodyType type, const std::string &texture, int layer = 1);

	// Cr�e un PolyChain
	void CreatePolyChain(const vector_b2Vec2 &vectices, PolyChain::Type type, const std::string &texture, int layer = 1);

	// Cr�e une PointLight
	void CreatePointLight(const b2Vec2 &pos, int lightRadius = 256, sf::Color lightColor = sf::Color::White, int layer = 1);

	// Cr�e un Ragdoll
	void CreateRagdoll(const b2Vec2 &position, int layer = 1);
}
