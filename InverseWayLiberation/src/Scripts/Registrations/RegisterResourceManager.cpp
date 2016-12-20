#include "../ScriptMachine.h"
#include "../../Resources/ResourceManager.h"

void LuaMachine::RegisterResourceManager()
{
	try
	{/*
		// Texture
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<Texture>("Texture")
				// Ctors
				.def(luabind::constructor<const std::string&, const std::string&>())
				// Attributs
				.property("name", &Texture::GetName)
				.property("path", &Texture::GetPath)
		];
	
		// ResourceManager
		luabind::module(mLuaState) [
			// Level
			luabind::namespace_("ResourceManager")
			[
				// Fxs
				luabind::def("LoadTexture", &ResourceManager::LoadTexture),
				luabind::def("GetTexture", &ResourceManager::GetTexture, luabind::dependency(luabind::result, _1))
			]
		];*/
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
/*
	// Gestion des textures
	bool LoadTexture(const std::string &name, const std::string &path);
	Texture::Ptr GetTexture(const std::string &name);

	// Obtient les maps
	const FontMap& GetFontMap();
	const SoundMap& GetSoundMap();
	const TextureMap& GetTextureMap();
*/