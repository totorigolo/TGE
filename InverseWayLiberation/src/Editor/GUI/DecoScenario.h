#pragma once
#include "Scenario.h"
#include "../../Entities/Deco.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class EditBox;
class DecoScenario : public Scenario
{
public:
	// Ctor
	DecoScenario(EditBox &editBox);

	// Gestion de la s�lection
	void Select(Deco *selection);
	void Unselect();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fen�tre et les �l�ments
	virtual void Fill();

	// Callbacks
	void OnChangePosition();
	void OnChangeTexture();
	void OnChangeLayer();
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
	ResourceManager &mResourceMgr;

	// S�lection
	Deco *mSelection;

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

	// Layer
	sfg::Box::Ptr mLayerHBox;
	sfg::Label::Ptr mLayerLabel;
	sfg::SpinButton::Ptr mLayer;

	// Texture
	sfg::Box::Ptr mTextureHBox;
	sfg::Label::Ptr mTextureLabel;
	sfg::ComboBox::Ptr mTexture;

	// Bouton
	sfg::Button::Ptr mRefresh;
	sfg::Button::Ptr mDestroy;
};
