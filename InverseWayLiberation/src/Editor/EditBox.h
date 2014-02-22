#pragma once
#include "GUI/LevelWindow.h"
#include "GUI/DecoScenario.h"
#include "GUI/EmptyScenario.h"
#include "GUI/LuaConsoleWindow.h"
#include "GUI/BodyScenario.h"
#include "GUI/ColFilteringWindow.h"
#include "GUI/PolyCreationWindow.h"
#include "../Entities/Entity.h"
#include "../Physics/Joint.h"
#include "../Lua/TriggersManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"

class EditBox : public NonCopyable
{
public:
	enum class SelectionType {
		Null,
		Entity,
			PolyBody,
			BasicBody,
			Deco,
			Grapnel,
			LivingBeing,
				Player,
		Joint,
			DistanceJoint,
			FrictionJoint,
			GearJoint,
			PrismaticJoint,
			PulleyJoint,
			RevoluteJoint,
			RopeJoint,
			WeldJoint,
			WheelJoint
	};
	std::string SelectionTypeToString(const SelectionType &type);

public:
	// Ctor & Dtor
	EditBox(sfg::Desktop &desktop);
	virtual ~EditBox();

	// Actions différées
	void DoScheduledTasks();
	void ScheduleUpdate();
	void ScheduleUnselection();

	// Change l'objet actuel
	void ChangeSelectedObject(Entity *entity);
	void ChangeSelectedObject(Joint *joint);
	void Unselect();
	SelectionType GetSelectionType();
	Entity* GetSelectedEntity();
	Joint* GetSelectedJoint();

	// Vide la EditBox
	void EmptyGUI();

	// Mise à jour des Widgets en f° de la sélection
	void UpdateGUI();

	// Gestion de la Machine Lua
	void SetLuaMachine(LuaMachine *luaMachine);
	LuaMachine* GetLuaMachine();

	// Obtient le repère de l'objet sélectionné
	sf::CircleShape GetSelectionMark();

	// Mise en place des différents scénarios
	void ShowEmptyScenario();
	void OnShowLevelWindow(); // Callback
	void ShowBodyScenario();
	void ShowDecoScenario();
	void OnShowLuaConsoleWindow(); // Callback
	void OnShowColFilteringWindow(); // Callback
	void ShowPolyCreationWindow();

	// Fermeture des fenêtres / scénarios
	void OnCloseLevelWindow(); // Callback

	// Accesseurs des fenêtres et scénarii
	EmptyScenario* GetEmptyScenario();
	LevelWindow* GetLevelWindow();
	DecoScenario* GetDecoScenario();
	BodyScenario* GetBodyScenario();
	LuaConsoleWindow* GetLuaConsoleWindow();
	ColFilteringWindow* GetColFilteringWindow();
	PolyCreationWindow* GetPolyCreationWindow();

private:
	// Actions différées
	bool mUpdateScheduled;
	bool mUnselectionScheduled;

	// Fenêtre
	sfg::Desktop &mDesktop;
	sfg::Window::Ptr mWindow;

	// LevelMgr, PhysicMgr, EntityManager
	LevelManager &mLevelMgr;
	PhysicManager &mPhysicMgr;
	EntityManager &mEntityMgr;

	// Objet sélectionné
	Entity *mSelectedEntity;
	Joint *mSelectedJoint;
	SelectionType mSelectionType;
	bool mSelectionChanged;

	// Différentes fenêtres / scénarios
	EmptyScenario mEmptyScenario;
	LevelWindow mLevelWindow;
	DecoScenario mDecoScenario;
	BodyScenario mBodyScenario;
	LuaConsoleWindow mLuaConsoleWindow;
	ColFilteringWindow mColFilteringWindow;
	PolyCreationWindow mPolyCreationWindow;

	// Etat des fenêtres / scénarios
	bool mLevelWindowAdded;
	bool mEditBoxGUICreated;
	bool mLuaConsoleWindowAdded;
	bool mColFilteringWindowAdded;
	bool mPolyCreationWindowAdded;
};
