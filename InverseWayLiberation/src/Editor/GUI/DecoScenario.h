#pragma once
#include "Scenario.h"
#include "../../Entities/Deco.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

class EditBox;
class DecoScenario : public Scenario
{
public:
	// Ctor
	DecoScenario(EditBox &editBox);

	// Gestion de la sélection
	void Select(Deco *selection);
	void Unselect();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

	// Callbacks
	void OnChangePosition();
	void OnChangePosXp();
	void OnChangePosXm();
	void OnChangePosYp();
	void OnChangePosYm();
	void OnChangePosRp();
	void OnChangePosRm();
	void OnDestroy();

private:
	// EntityManager, PhysicManager
	EntityManager &mEntityMgr;
	PhysicManager &mPhysicMgr;

	// Sélection
	Deco *mSelection;

	/* Eléments */
	// Position
	sfg::Table::Ptr mPosTable;
	sfg::Label::Ptr mPosLabel;
	sfg::Entry::Ptr mPosX;
	sfg::Entry::Ptr mPosY;
	sfg::Entry::Ptr mRot;
	sfg::Button::Ptr mPosButton;
	sfg::SpinButton::Ptr mPosStep;
	float mPosStepSaveValue;
	sfg::Button::Ptr mPosXp;
	sfg::Button::Ptr mPosXm;
	sfg::Button::Ptr mPosYp;
	sfg::Button::Ptr mPosYm;
	sfg::Button::Ptr mPosRp;
	sfg::Button::Ptr mPosRm;

	// Bouton
	sfg::Button::Ptr mDestroy;
};
