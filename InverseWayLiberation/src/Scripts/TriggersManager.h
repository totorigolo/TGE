#pragma once

#include <list>
#include "../Tools/Singleton.h"
#include "../Lua/LuaMachine.h"
#include "ScriptAction.h"
#include "ScriptArea.h"

typedef std::map<std::string, std::shared_ptr<LuaAction>> ActionMap;

class PhysicManager;
class TriggersManager : public Singleton<TriggersManager>
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
	std::shared_ptr<LuaAction> GetAction(const std::string &name);
	ActionMap& GetActionMap();
	const ActionMap& GetActionMap() const;

	// Gère les Areas
	void CreateArea(b2AABB area, const std::string &action, bool once = false);
	void DestroyArea(LuaArea *area);
	std::list<std::shared_ptr<LuaArea>>& GetAreas();
	const std::list<std::shared_ptr<LuaArea>>& GetAreas() const;

	// Suppressions planifiées
	void ScheduleRemove(std::shared_ptr<LuaArea> area);
	void ScheduleRemove(const std::string &action);

	// Gestion de la machine Lua
	LuaMachine* GetLuaMachine();
	void SetLuaMachine(LuaMachine *luaMachine);

	// Affichage debug
	void DebugDraw(sf::RenderTarget &target) const;

private:
	// Liste d'actions
	ActionMap mActionMap;

	// Liste de zones
	std::list<std::shared_ptr<LuaArea>> mAreas;

	// Machine Lua
	LuaMachine *mLuaMachine;

	// PhysicManager
	PhysicManager &mPhysicMgr;

	// Suppressions planifiées
	std::list<std::weak_ptr<LuaArea>> mAreasToDelete;
	std::list<std::string> mActionsToDelete;
};
