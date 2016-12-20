#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Canvas.hpp>
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
class TexturesWindow : public Window
{
public:
	// Ctor
	TexturesWindow();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

	// Callbacks
	void OnLoadTexture();
	void OnSelectTexture();

private:
	// EntityManager, ResourceManager
	EntityManager &mEntityMgr;
	ResourceManager &mResourceMgr;

	// Texture sélectionnée
	std::weak_ptr<Texture> mSelectedTexture;

	/* Eléments */
	sfg::Box::Ptr mVBox;
	sfg::Label::Ptr mInfosSectionLabel;

	// Chargement de texture
	sfg::Box::Ptr mNameHBox;
	sfg::Label::Ptr mNameLabel;
	sfg::Entry::Ptr mName;
	sfg::Box::Ptr mPathHBox;
	sfg::Label::Ptr mPathLabel;
	sfg::Entry::Ptr mPath;
	sfg::Button::Ptr mLoadBtn;
	sfg::Label::Ptr mLoadLabel;

	// Texture
	sfg::Box::Ptr mTextureHBox;
	sfg::Label::Ptr mTextureLabel;
	sfg::ComboBox::Ptr mTexture;

	// Infos texture
	sfg::Label::Ptr mNbUsesLabel;
	sfg::Canvas::Ptr mCanvas;

	// Boutons
	sfg::Box::Ptr mButtonsHBox;
	sfg::Button::Ptr mRefreshBtn;
	sfg::Button::Ptr mCloseBtn;
};
