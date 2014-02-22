#pragma once

namespace EntityFactory
{
	// Charge une texture
	void LoadTexture(const std::string &name, const std::string &path);

	// Crée une déco
	void CreateDeco(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Crée une DynamicBox
	void CreateDynamicBox(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Crée un StaticBox
	void CreateStaticBox(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Crée un DynamicCircle
	void CreateDynamicCircle(const b2Vec3 &posRot, const std::string &texture, int layer = 1);

	// Crée un PolyBody
	void CreatePolyBody(const std::vector<b2Vec2> &vectices, const b2BodyType &type, const std::string &texture, int layer = 1);

	// Crée un lampadaire
	void CreateLamp(const b2Vec3 &posRot, int layer = 1);

	// Crée un Ragdoll
	void CreateRagdoll(const b2Vec2 &position, int layer = 1);
}
