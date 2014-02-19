#pragma once
#include "Window.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

class LevelWindow : public Window
{
public:
	// Ctor
	LevelWindow();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fen�tre et les �l�ments
	virtual void Fill();

	// Callbacks
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
	// LevelMgr, PhysicMgr
	LevelManager &mLevelMgr;
	PhysicManager &mPhysicMgr;

	// El�ments
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
};
