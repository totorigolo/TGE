#include "../ScriptMachine.h"
#include "../../Resources/ResourceManager.h"

void ScriptMachine::RegisterResourceManager() {
    try {
        // Texture
        luabind::module(mLuaState)
        [
                // La classe
                luabind::class_<Texture>("Texture")
                        // Ctors
                        .def(luabind::constructor<const std::string &, const std::string &>())
                                // Attributs
                        .property("name", &Texture::GetName)
                        .property("path", &Texture::GetPath)
        ];

        // InputManager
        luabind::module(mLuaState)
        [
                luabind::class_<ResourceManager>("ResourceManager")
                        /* Fxs */
                        .def("LoadTexture", &ResourceManager::LoadTexture)
                        .def("GetTexture", &ResourceManager::GetTexture, luabind::dependency_policy<0, 1>())
//                                             luabind::dependency(luabind::result, _1))
        ];
    }
    catch (const std::exception &e) {
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