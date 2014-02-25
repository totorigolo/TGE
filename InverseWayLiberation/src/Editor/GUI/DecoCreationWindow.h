#pragma once
#include "Window.h"
#include "../../Entities/BasicBody.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class EditBox;
class DecoCreationWindow : public Window
{
public:
	// Ctor
	DecoCreationWindow();

	// Gestion de l'ajout
	void Add(b2Vec2 pos);

	// Actualisation
	virtual void Update();

	// Accesseurs
	bool IsInAddMode();

protected:
	// Construit la fen�tre et les �l�ments
	virtual void Fill();

	// Callbacks
	void OnToggleMode();

private:
	// Etat
	bool mIsInAddMode;

	// EntityManager, PhysicMgr
	EntityManager &mEntityMgr;
	PhysicManager &mPhysicMgr;
	ResourceManager &mResourceMgr;

	/* El�ments */
	sfg::Box::Ptr mVBox;
	sfg::Label::Ptr mMode;
	sfg::Label::Ptr mHelpLabel;

	// Texture
	sfg::Box::Ptr mTextureHBox;
	sfg::Label::Ptr mTextureLabel;
	sfg::ComboBox::Ptr mTexture;

	// Layer
	sfg::Box::Ptr mLayerHBox;
	sfg::Label::Ptr mLayerLabel;
	sfg::SpinButton::Ptr mLayer;

	// Boutons
	sfg::Button::Ptr mToggleModeBtn;
	sfg::Box::Ptr mButtonsHBox;
	sfg::Button::Ptr mRefreshBtn;
	sfg::Button::Ptr mCloseBtn;
};
