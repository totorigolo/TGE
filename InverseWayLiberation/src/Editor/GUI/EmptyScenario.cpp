#include "EmptyScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Tools/Error.h"
#include "../../Tools/utils.h"

// Ctor
EmptyScenario::EmptyScenario(EditBox &editBox)
	: Scenario(editBox, "EditBox")
{
	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Actualisation
void EmptyScenario::Update()
{
}

// Construit la fenêtre et les éléments
void EmptyScenario::Fill()
{
	// Widgets
	mOpenLevelBtn = sfg::Button::Create("Fenêtre Level");
	mOpenLevelBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowLevelWindow, &mEditBox));

	mOpenLuaConsoleBtn = sfg::Button::Create("Console Lua");
	mOpenLuaConsoleBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowLuaConsoleWindow, &mEditBox));

	mPolyCreationBtn = sfg::Button::Create("Poly Creation");
	mPolyCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowPolyCreationWindow, &mEditBox));

	// Ajoute les éléments à la fenêtre
	AddToVBox(mOpenLevelBtn);
	AddToVBox(mOpenLuaConsoleBtn);
	AddToVBox(mPolyCreationBtn);
}
