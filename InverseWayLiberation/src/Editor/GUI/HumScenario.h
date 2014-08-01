#pragma once
#include "Scenario.h"
#include "../../Entities/Hum.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"
#include "../../Resources/ResourceManager.h"

class EditBox;
class HumScenario : public Scenario
{
public:
	// Ctor
	HumScenario(EditBox &editBox);

	// Gestion de la sélection
	void Select(Hum *selection);
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
	void OnChangeColor();
	void OnChangeLayer();
	void OnClearForces();
	void OnDestroy();

private:
	// EntityManager, TexturesManager
	EntityManager &mEntityMgr;
	ResourceManager &mResourceMgr;

	// Sélection
	Hum *mSelection;

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

	// Couleur
	sfg::Box::Ptr mColorHBox;
	sfg::Label::Ptr mColorLabel;
	sfg::SpinButton::Ptr mColorR;
	sfg::SpinButton::Ptr mColorG;
	sfg::SpinButton::Ptr mColorB;

	// Layer
	sfg::Box::Ptr mLayerHBox;
	sfg::Label::Ptr mLayerLabel;
	sfg::SpinButton::Ptr mLayer;

	// Boutons
	sfg::Box::Ptr mButtonsHBox1;
	sfg::Box::Ptr mButtonsHBox2;
	sfg::Button::Ptr mRefresh;
	sfg::Button::Ptr mDestroy;
	sfg::Button::Ptr mClearForces;
};
