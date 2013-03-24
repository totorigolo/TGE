#pragma once
#include <Box2D/Box2D.h>
#include <string>
#include "../Physics/PhysicManager.h"

namespace EntityFactory
{
	// Parametres de la Factory
	void SetPhysicManager(PhysicManager *physicMgr);

	// Cr�e un Ragdoll
	void CreateRagdoll(const b2Vec2 &position, int layer = 1);
	
	// Cr�e une Box
	void CreateBox(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Cr�e un cercle
	void CreateCircle(const b2Vec3 &posRot, const std::string &texture, int layer = 1);

	// Cr�e un lampadaire
	void CreateLamp(const b2Vec3 &posRot, int layer = 1);
}
