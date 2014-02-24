#include "stdafx.h"
#include "../LuaMachine.h"
#include "../../Tools/vector_wrappers.h"

// Enregistrements privés
void LuaMachine::RegisterBox2D()
{
	/* Enregistre les classes */
	try
	{
		// b2BodyType
		luabind::globals(mLuaState)["b2_dynamicBody"] = b2BodyType::b2_dynamicBody;
		luabind::globals(mLuaState)["b2_staticBody"] = b2BodyType::b2_staticBody;
		luabind::globals(mLuaState)["b2_kinematicBody"] = b2BodyType::b2_kinematicBody;

		// b2Vec2
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<b2Vec2>("b2Vec2")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<float32, float32>())
				// Fxs
				.def("SetZero", &b2Vec2::SetZero)
				.def("Set", &b2Vec2::Set)
				.def("Length", &b2Vec2::Length)
				.def("LengthSquared", &b2Vec2::LengthSquared)
				.def("Normalize", &b2Vec2::Normalize)
				.def("IsValid", &b2Vec2::IsValid)
				.def("Skew", &b2Vec2::Skew)
				// Ops
				.def(- luabind::const_self)
				.def(float32() * luabind::const_self)
				.def(luabind::const_self + b2Vec2())
				.def(luabind::const_self - b2Vec2())
				.def(luabind::const_self == b2Vec2())
				// Attributs
				.def_readwrite("x", &b2Vec2::x)
				.def_readwrite("y", &b2Vec2::y)
				,
			// Fxs
			luabind::def("b2Dot", (float32(*)(const b2Vec2&, const b2Vec2&)) b2Dot),
			luabind::def("b2Cross", (float32(*)(const b2Vec2&, const b2Vec2&)) b2Cross),
			luabind::def("b2Cross", (b2Vec2(*)(const b2Vec2&, float32)) b2Cross),
			luabind::def("b2Cross", (b2Vec2(*)(float32, const b2Vec2&)) b2Cross),
			luabind::def("b2Distance", (float32(*)(const b2Vec2&, const b2Vec2&)) b2Distance),
			luabind::def("b2DistanceSquared", (float32(*)(const b2Vec2&, const b2Vec2&)) b2DistanceSquared),

			// vector_b2Vec2
			REGISTER_VECTOR_WRAPPER(b2Vec2, vector_b2Vec2)
		];

		// b2Vec3
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<b2Vec3>("b2Vec3")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<float32, float32, float32>())
				// Fxs
				.def("SetZero", &b2Vec3::SetZero)
				.def("Set", &b2Vec3::Set)
				// Ops
				.def(- luabind::const_self)
				.def(float32() * luabind::const_self)
				.def(luabind::const_self + b2Vec3())
				.def(luabind::const_self - b2Vec3())
				// Attributs
				.def_readwrite("x", &b2Vec3::x)
				.def_readwrite("y", &b2Vec3::y)
				.def_readwrite("z", &b2Vec3::y)
				,
			// Fxs
			luabind::def("b2Dot", (float32(*)(const b2Vec3&, const b2Vec3&)) b2Dot),
			luabind::def("b2Cross", (b2Vec3(*)(const b2Vec3&, const b2Vec3&)) b2Cross)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
