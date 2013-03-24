#pragma once
#include <Box2D/Box2D.h>
#include <string>
#include "../Physics/PhysicManager.h"

namespace EntityFactory
{
	// Parametres de la Factory
	void SetPhysicManager(PhysicManager *physicMgr);

	// Crée un Ragdoll
	void CreateRagdoll(const b2Vec2 &position, int layer = 1);
	
	// Crée une Box
	void CreateBox(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Crée un cercle
	void CreateCircle(const b2Vec3 &posRot, const std::string &texture, int layer = 1);

	// Crée un lampadaire
	void CreateLamp(const b2Vec3 &posRot, int layer = 1);
}
