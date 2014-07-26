#pragma once
#include "Scenario.h"
#include "../../Entities/PointLight.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class EditBox;
class PointLightScenario : public Scenario
{
public:
	// Ctor
	PointLightScenario(EditBox &editBox);

	// Gestion de la s�lection
	void Select(PointLight *selection);
	void Unselect();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fen�tre et les �l�ments
	virtual void Fill();

	// Callbacks
	void OnChangePosition();
	void OnChangeLayer();
	void OnChangePosXp();
	void OnChangePosXm();
	void OnChangePosYp();
	void OnChangePosYm();
	void OnChangeRadius();
	void OnChangeColorR();
	void OnChangeColorG();
	void OnChangeColorB();
	void OnDestroy();

private:
	// EntityManager, PhysicManager
	EntityManager &mEntityMgr;
	PhysicManager &mPhysicMgr;

	// S�lection
	PointLight *mSelection;

	/* El�ments */
	// Position
	sfg::Table::Ptr mPosTable;
	sfg::Label::Ptr mPosLabel;
	sfg::Entry::Ptr mPosX;
	sfg::Entry::Ptr mPosY;
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

	// Rayon
	sfg::Box::Ptr mRadiusHBox;
	sfg::Label::Ptr mRadiusLabel;
	sfg::SpinButton::Ptr mRadius;

	// Layer
	sfg::Box::Ptr mLayerHBox;
	sfg::Label::Ptr mLayerLabel;
	sfg::SpinButton::Ptr mLayer;

	// Bouton
	sfg::Button::Ptr mRefresh;
	sfg::Button::Ptr mDestroy;
};
