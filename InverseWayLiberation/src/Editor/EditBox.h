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

	// Actions diff�r�es
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

	// Mise � jour des Widgets en f� de la s�lection
	void UpdateGUI();

	// Gestion de la Machine Lua
	void SetLuaMachine(LuaMachine *luaMachine);
	LuaMachine* GetLuaMachine();

	// Obtient le rep�re de l'objet s�lectionn�
	sf::CircleShape GetSelectionMark();

	// Mise en place des diff�rents sc�narios
	void ShowEmptyScenario();
	void OnShowLevelWindow(); // Callback
	void ShowBodyScenario();
	void ShowDecoScenario();
	void OnShowLuaConsoleWindow(); // Callback
	void OnShowColFilteringWindow(); // Callback
	void ShowPolyCreationWindow();

	// Fermeture des fen�tres / sc�narios
	void OnCloseLevelWindow(); // Callback

	// Accesseurs des fen�tres et sc�narii
	EmptyScenario* GetEmptyScenario();
	LevelWindow* GetLevelWindow();
	DecoScenario* GetDecoScenario();
	BodyScenario* GetBodyScenario();
	LuaConsoleWindow* GetLuaConsoleWindow();
	ColFilteringWindow* GetColFilteringWindow();
	PolyCreationWindow* GetPolyCreationWindow();

private:
	// Actions diff�r�es
	bool mUpdateScheduled;
	bool mUnselectionScheduled;

	// Fen�tre
	sfg::Desktop &mDesktop;
	sfg::Window::Ptr mWindow;

	// LevelMgr, PhysicMgr, EntityManager
	LevelManager &mLevelMgr;
	PhysicManager &mPhysicMgr;
	EntityManager &mEntityMgr;

	// Objet s�lectionn�
	Entity *mSelectedEntity;
	Joint *mSelectedJoint;
	SelectionType mSelectionType;
	bool mSelectionChanged;

	// Diff�rentes fen�tres / sc�narios
	EmptyScenario mEmptyScenario;
	LevelWindow mLevelWindow;
	DecoScenario mDecoScenario;
	BodyScenario mBodyScenario;
	LuaConsoleWindow mLuaConsoleWindow;
	ColFilteringWindow mColFilteringWindow;
	PolyCreationWindow mPolyCreationWindow;

	// Etat des fen�tres / sc�narios
	bool mLevelWindowAdded;
	bool mEditBoxGUICreated;
	bool mLuaConsoleWindowAdded;
	bool mColFilteringWindowAdded;
	bool mPolyCreationWindowAdded;
};
