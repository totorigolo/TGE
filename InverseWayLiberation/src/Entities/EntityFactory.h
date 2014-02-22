#pragma once

namespace EntityFactory
{
	// Charge une texture
	void LoadTexture(const std::string &name, const std::string &path);

	// Cr�e une d�co
	void CreateDeco(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Cr�e une DynamicBox
	void CreateDynamicBox(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Cr�e un StaticBox
	void CreateStaticBox(const b2Vec3 &posRot, const std::string &texture, int layer = 1);
	
	// Cr�e un DynamicCircle
	void CreateDynamicCircle(const b2Vec3 &posRot, const std::string &texture, int layer = 1);

	// Cr�e un PolyBody
	void CreatePolyBody(const std::vector<b2Vec2> &vectices, const b2BodyType &type, const std::string &texture, int layer = 1);

	// Cr�e un lampadaire
	void CreateLamp(const b2Vec3 &posRot, int layer = 1);

	// Cr�e un Ragdoll
	void CreateRagdoll(const b2Vec2 &position, int layer = 1);
}
