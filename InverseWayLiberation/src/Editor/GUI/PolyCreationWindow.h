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
#include "../../Entities/BasicBody.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class EditBox;
class PolyCreationWindow : public Window
{
public:
	// Ctor
	PolyCreationWindow();

	// Gestion de l'ajout de points
	void AddPoint(b2Vec2 pos);
	void EmptyPoints();
	void DrawPointsOn(sf::RenderTarget &target, const sf::RenderStates& states = sf::RenderStates::Default);

	// Actualisation
	virtual void Update();

	// Accesseurs
	bool IsInEditMode();

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

	// Callbacks
	void OnEmptyPoints();
	void OnCancelLast();
	void OnCreatePoly();
	void OnToggleEditMode();
	void OnTypeClicked();

private:
	// Etat
	bool mIsEnterEditMode;

	// EntityManager, PhysicMgr
	EntityManager &mEntityMgr;
	PhysicManager &mPhysicMgr;
	ResourceManager &mResourceMgr;

	// Points
	std::vector<b2Vec2> mPoints;

	/* Eléments */
	sfg::Box::Ptr mVBox;
	sfg::Label::Ptr mMode;
	sfg::Label::Ptr mHelpLabel;

	// Type
	sfg::Box::Ptr mTypeHBox;
	sfg::Label::Ptr mTypeLabel;
	std::vector<sfg::RadioButton::Ptr> mType;

	// Type de Body
	sfg::Box::Ptr mTypeBodyHBox;
	sfg::Label::Ptr mTypeBodyLabel;
	std::vector<sfg::RadioButton::Ptr> mTypeBody;

	// Texture
	sfg::Box::Ptr mTextureHBox;
	sfg::Label::Ptr mTextureLabel;
	sfg::ComboBox::Ptr mTexture;

	// Layer
	sfg::Box::Ptr mLayerHBox;
	sfg::Label::Ptr mLayerLabel;
	sfg::SpinButton::Ptr mLayer;

	// Boutons
	sfg::Button::Ptr mEnterEditModeBtn;
	sfg::Button::Ptr mCreatePolyBtn;
	sfg::Box::Ptr mButtonsHBox;
	sfg::Button::Ptr mCloseBtn;
	sfg::Button::Ptr mCancelBtn;
	sfg::Button::Ptr mEmptyBtn;
};
