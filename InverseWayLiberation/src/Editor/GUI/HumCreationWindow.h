#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/SpinButton.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/ComboBox.hpp>
#include "Window.h"
#include "../../Entities/Hum.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"
#include "../../Resources/ResourceManager.h"

class EditBox;
class HumCreationWindow : public Window
{
public:
	// Ctor
	HumCreationWindow();

	// Gestion de l'ajout
	void Add(b2Vec2 pos);

	// Actualisation
	virtual void Update();

	// Accesseurs
	bool IsInAddMode();

protected:
	// Construit la fenêtre et les éléments
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

	/* Eléments */
	sfg::Box::Ptr mVBox;
	sfg::Label::Ptr mMode;
	sfg::Label::Ptr mHelpLabel;

	// Age
	sfg::Box::Ptr mAgeHBox;
	sfg::Label::Ptr mAgeLabel;
	sfg::SpinButton::Ptr mAge;

	// Force
	sfg::Box::Ptr mStrenghHBox;
	sfg::Label::Ptr mStrenghLabel;
	sfg::SpinButton::Ptr mStrengh;

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

	// Player
	sfg::CheckButton::Ptr mIsPlayer;

	// Boutons
	sfg::Button::Ptr mToggleModeBtn;
	sfg::Button::Ptr mCloseBtn;
};
