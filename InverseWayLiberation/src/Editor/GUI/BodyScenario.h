#pragma once
#include "Scenario.h"
#include "../../Entities/PolyBody.h"
#include "../../Entities/BasicBody.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

// TODO: Cr�er un classe de base commune � BasicBody et � PolyBody pour �viter tout le code dupliqu�

class EditBox;
class BodyScenario : public Scenario
{
public:
	// Ctor
	BodyScenario(EditBox &editBox);

	// Gestion de la s�lection
	void Select(PolyBody *selection);
	void Select(BasicBody *selection);
	void Unselect();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fen�tre et les �l�ments
	virtual void Fill();

	// Callbacks
	void OnChangePosition();
	void OnChangeType();
	void OnChangeCollisionType();
	void OnChangePosXp();
	void OnChangePosXm();
	void OnChangePosYp();
	void OnChangePosYm();
	void OnChangePosRp();
	void OnChangePosRm();
	void OnChangeDensity();
	void OnChangeFriction();
	void OnChangeRestitution();
	void OnClearForces();
	void OnDestroy();

private:
	// EntityManager
	EntityManager &mEntityMgr;

	// S�lection
	PolyBody *mSelectionPoly;
	BasicBody *mSelectionBasic;

	/* El�ments */
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

	// Type
	sfg::Table::Ptr mTypeTable;
	sfg::Label::Ptr mTypeLabel;
	std::vector<sfg::RadioButton::Ptr> mType;

	// CollisionType
	sfg::Table::Ptr mCollisionTypeTable;
	sfg::Label::Ptr mCollisionTypeLabel;
	sfg::ComboBox::Ptr mCollisionType;

	// Densit�, friction, restitution
	sfg::Table::Ptr mPhysicsParamsTable;
	sfg::Label::Ptr mDensityLabel;
	sfg::SpinButton::Ptr mDensity;
	sfg::Label::Ptr mFrictionLabel;
	sfg::SpinButton::Ptr mFriction;
	sfg::Label::Ptr mRestitutionLabel;
	sfg::SpinButton::Ptr mRestitution;

	// Boutons
	sfg::Box::Ptr mButtonsHBox1;
	sfg::Box::Ptr mButtonsHBox2;
	sfg::Button::Ptr mRefresh;
	sfg::Button::Ptr mDestroy;
	sfg::Button::Ptr mClearForces;
	sfg::Button::Ptr mColFilteringButton;
};