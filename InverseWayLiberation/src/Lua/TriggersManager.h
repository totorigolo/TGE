#pragma once
#include "../Lua/LuaMachine.h"
#include "LuaAction.h"

typedef std::unordered_map<std::string, std::shared_ptr<LuaAction>> ActionMap;

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
	ActionMap& GetActionMap();
	const ActionMap& GetActionMap() const;

	// Gère les Areas
	void CreateArea(b2AABB area, const std::string &action);
	// TODO: DestroyArea
	std::list<std::pair<b2AABB, std::string>>& GetAreas();
	const std::list<std::pair<b2AABB, std::string>>& GetAreas() const;

	// Gestion de la machine Lua
	LuaMachine* GetLuaMachine();
	void SetLuaMachine(LuaMachine *luaMachine);

	// Affichage debug
	void DebugDraw(sf::RenderTarget &target) const;

private:
	// Liste d'actions
	ActionMap mActionMap;

	// Liste de zones
	// TODO: Créer une table de correspondance string <> unsigned int
	std::list<std::pair<b2AABB, std::string>> mAreas; // TODO: multi-map

	// Machine Lua
	LuaMachine *mLuaMachine;

	// PhysicManager
	PhysicManager &mPhysicMgr;
};
