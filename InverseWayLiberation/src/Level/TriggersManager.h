#pragma once
#include "../Game/LuaMachine.h"
#include "../Tools/NonCopyable.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <map>

class TriggersManager;
class LuaAction
{
public:
	// Ctors
	LuaAction(const std::string &_name, const std::string &_file)
		: mName(_name), mFile(_file)
	{
	}
	LuaAction(const std::string &_name, const std::string &_file, std::string &_function)
		: mName(_name), mFile(_file), mFunction(_function)
	{
	}

	// Exécute l'action
	void Execute(LuaMachine *luaMachine)
	{
		// Si on a une fonction, on charge le fichier et exécute la fx
		if (HasFunction())
		{
			luaMachine->DoFile(mFile);
			
			try
			{
				luabind::call_function<void>(luaMachine->GetLuaState(), mFunction.c_str());
			}
			catch (const std::exception &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}

		// Sinon on exécute le fichier
		else
		{
			luaMachine->DoFile(mFile);
		}
	}

	// Accesseurs
	bool HasFunction() { return !mFunction.empty(); }
	const std::string& GetName() { return mName; }
	const std::string& GetFile() { return mFile; }
	const std::string& GetFunction() { return mFunction; }

private:
	std::string mName;
	std::string mFile;
	std::string mFunction;
};

class PhysicManager;
class TriggersManager : public NonCopyable
{
public:
	// Ctor & dtor
	TriggersManager();
	virtual ~TriggersManager(void);

	// Mise à jour
	void Update();

	// Vidage
	void Clear();
	
	// Gestion des actions
	// TODO: ActionManager
	void AddAction(LuaAction *action);
	void DeleteAction(LuaAction *action);
	void DeleteAction(const std::string &name);

	// Gère les Areas
	void CreateArea(b2AABB area, const std::string &action);
	// TODO: DestroyArea

	// Gestion de la machine Lua
	LuaMachine* GetLuaMachine();
	void SetLuaMachine(LuaMachine *luaMachine);

	// Affichage debug
	void DebugDraw(sf::RenderTarget &target) const;

private:
	// Liste d'actions
	std::map<std::string, std::shared_ptr<LuaAction>> mActionMap;

	// Liste de zones
	// TODO: Créer une table de correspondance string <> unsigned int
	std::list<std::pair<b2AABB, std::string>> mAreas; // TODO: multi-map

	// Machine Lua
	LuaMachine *mLuaMachine;

	// PhysicManager
	PhysicManager &mPhysicMgr;
};
