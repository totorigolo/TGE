#pragma once
#include "../Physics/PhysicManager.h"
#include <Box2D/Box2D.h>
#include <string>

namespace EntityFactory
{
	// Cr�e un Ragdoll
	void CreateRagdoll(PhysicManager *physicMgr, const b2Vec2 &position, int layer = 1);
	
	// Cr�e une Box parmi la liste
	void CreateBox(PhysicManager *physicMgr, const b2Vec3 &posRot, std::string list[], int list_size, int layer = 1);
	
	// Cr�e un cercle parmi la liste
	void CreateCircle(PhysicManager *physicMgr, const b2Vec3 &posRot, std::string list[], int list_size, int layer = 1);

	// Cr�e un lampadaire
	void CreateLamp(PhysicManager *physicMgr, const b2Vec3 &posRot, int layer = 1);
}
