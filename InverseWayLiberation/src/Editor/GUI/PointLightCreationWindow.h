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
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class EditBox;
class PointLightCreationWindow : public Window
{
public:
	// Ctor
	PointLightCreationWindow();

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

	/* Eléments */
	sfg::Box::Ptr mVBox;
	sfg::Label::Ptr mMode;
	sfg::Label::Ptr mHelpLabel;

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

	// Boutons
	sfg::Button::Ptr mToggleModeBtn;
	sfg::Box::Ptr mButtonsHBox;
	sfg::Button::Ptr mRefreshBtn;
	sfg::Button::Ptr mCloseBtn;
};
