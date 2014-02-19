#pragma once
#include "Window.h"
#include "../../Entities/BasicBody.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

class ColFilteringWindow : public Window
{
public:
	// Ctor
	ColFilteringWindow();

	// Gestion de la sélection
	void Select(BasicBody *selection);
	void Unselect();

	// Actualisation
	virtual void Update();

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

	// Callbacks
	void OnColFilteringApply();
	void OnColFilteringRefresh();
	void OnColFilteringClose();

private:
	// LevelMgr, PhysicMgr
	LevelManager &mLevelMgr;
	PhysicManager &mPhysicMgr;

	// Sélection
	BasicBody *mSelection;

	// Eléments
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
