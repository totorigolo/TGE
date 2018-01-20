#pragma once

#include <list>
#include "../Tools/Singleton.h"
#include "../Scripts/ScriptMachine.h"
#include "ScriptAction.h"
#include "ScriptArea.h"

typedef std::map<std::string, std::shared_ptr<ScriptAction>> ActionMap;

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
	void AddAction(ScriptAction *action);
	void DeleteAction(ScriptAction *action);
	void DeleteAction(const std::string &name);
	std::shared_ptr<ScriptAction> GetAction(const std::string &name);
	ActionMap& GetActionMap();
	const ActionMap& GetActionMap() const;

	// Gère les Areas
	void CreateArea(b2AABB area, const std::string &action, bool once = false);
	void DestroyArea(ScriptArea *area);
	std::list<std::shared_ptr<ScriptArea>>& GetAreas();
	const std::list<std::shared_ptr<ScriptArea>>& GetAreas() const;

	// Suppressions planifiées
	void ScheduleRemove(std::shared_ptr<ScriptArea> area);
	void ScheduleRemove(const std::string &action);

	// Gestion de la machine de script
	ScriptMachine* GetScriptMachine();
	void SetScriptMachine(ScriptMachine *luaMachine);

	// Affichage debug
	void DebugDraw(sf::RenderTarget &target) const;

private:
	// Liste d'actions
	ActionMap mActionMap;

	// Liste de zones
	std::list<std::shared_ptr<ScriptArea>> mAreas;

	// Machine de script
	ScriptMachine *mScriptMachine;

	// PhysicManager
	PhysicManager &mPhysicMgr;

	// Suppressions planifiées
	std::list<std::weak_ptr<ScriptArea>> mAreasToDelete;
	std::list<std::string> mActionsToDelete;
};
