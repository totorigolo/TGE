#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/SpinButton.hpp>
#include <SFGUI/Button.hpp>
#include "Window.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class Editor;
class EditBox;
class ScriptMachine;
class LevelWindow : public Window
{
public:
	// Ctor
	LevelWindow();

	// Actualisation
	virtual void Update();

	// Enregistrements
	void SetEditor(Editor *editor);
	void SetEditBox(EditBox *editBox);
	void SetLuaMachine(ScriptMachine *luaMachine);

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

	// Callbacks
	void OnLoad();
	void OnSave();
	void OnChangeGravityX();
	void OnChangeGravityY();
	void OnChangeBckgColorR();
	void OnChangeBckgColorG();
	void OnChangeBckgColorB();
	void OnChangeOriginViewX();
	void OnChangeOriginViewY();
	void OnChangeOriginViewCurrent();
	void OnChangeDefaultZoom();
	void OnChangeDefaultZoomCurrent();

private:
	// LevelMgr, PhysicMgr, ResourceMgr
	LevelManager &mLevelMgr;
	PhysicManager &mPhysicMgr;
	ResourceManager &mResourceManager;

	// Enregistrements
	Editor *mEditor;
	EditBox *mEditBox;
	ScriptMachine *mLuaMachine;

	// Eléments
	sfg::Box::Ptr mVBox;
	sfg::Box::Ptr mGravityBox;
	sfg::Label::Ptr mGravityLabel;
	sfg::SpinButton::Ptr mGravityX;
	sfg::SpinButton::Ptr mGravityY;
	sfg::Box::Ptr mBckgColorBox;
	sfg::Label::Ptr mBckgColorLabel;
	sfg::SpinButton::Ptr mBckgColorR;
	sfg::SpinButton::Ptr mBckgColorG;
	sfg::SpinButton::Ptr mBckgColorB;
	sfg::Box::Ptr mOriginViewBox;
	sfg::Label::Ptr mOriginViewLabel;
	sfg::SpinButton::Ptr mOriginViewX;
	sfg::SpinButton::Ptr mOriginViewY;
	sfg::Button::Ptr mOriginViewCurrentBtn;
	sfg::Box::Ptr mDefaultZoomBox;
	sfg::Label::Ptr mDefaultZoomLabel;
	sfg::SpinButton::Ptr mDefaultZoom;
	sfg::Button::Ptr mDefaultZoomCurrentBtn;
	sfg::Box::Ptr mButtonBox;
	sfg::Button::Ptr mRefreshBtn;
	sfg::Button::Ptr mCloseBtn;

	// Chargement et sauvegarde
	sfg::Box::Ptr mPathHBox;
	sfg::Label::Ptr mPathLabel;
	sfg::Entry::Ptr mPath;
	sfg::Box::Ptr mSaveLoadHBox;
	sfg::Button::Ptr mLoadBtn;
	sfg::Button::Ptr mSaveBtn;
};
