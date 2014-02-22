#pragma once
#include "Window.h"
#include "../../Entities/BaseBody.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class ColFilteringWindow : public Window
{
public:
	// Ctor
	ColFilteringWindow();

	// Gestion de la s�lection
	void Select(BaseBody *selection);
	void Unselect();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fen�tre et les �l�ments
	virtual void Fill();

	// Callbacks
	void OnColFilteringApply();
	void OnColFilteringRefresh();
	void OnColFilteringClose();

private:
	// LevelMgr, PhysicMgr
	LevelManager &mLevelMgr;
	PhysicManager &mPhysicMgr;

	// S�lection
	BaseBody *mSelection;

	// El�ments
	sfg::Window::Ptr mColFilteringWindow;
	sfg::Box::Ptr mColFilteringVBox;
	sfg::Box::Ptr mGroupIndexBox;
	sfg::Label::Ptr mGroupIndexLabel;
	sfg::SpinButton::Ptr mGroupIndex;
	sfg::Label::Ptr mCatBitsLabel;
	sfg::Label::Ptr mMaskBitsLabel;
	std::vector<sfg::ToggleButton::Ptr> mCatBits;
	std::vector<sfg::ToggleButton::Ptr> mMaskBits;
	sfg::Table::Ptr mBitsTable;
	sfg::Box::Ptr mColFilteringButtonsHBox;
	sfg::Button::Ptr mColFilteringApply;
	sfg::Button::Ptr mColFilteringRefresh;
	sfg::Button::Ptr mColFilteringClose;
};
