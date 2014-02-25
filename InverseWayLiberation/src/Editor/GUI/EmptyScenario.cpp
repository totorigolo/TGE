#include "stdafx.h"
#include "EmptyScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"

// Ctor
EmptyScenario::EmptyScenario(EditBox &editBox)
	: Scenario(editBox, "EditBox")
{
	// Rempli la fen�tre
	Fill();
	mApply = true;
}

// Actualisation
void EmptyScenario::Update()
{
}

// Construit la fen�tre et les �l�ments
void EmptyScenario::Fill()
{
	// Widgets
	mOpenLevelBtn = sfg::Button::Create("Fen�tre Level");
	mOpenLevelBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowLevelWindow, &mEditBox));

	mOpenLuaConsoleBtn = sfg::Button::Create("Console Lua");
	mOpenLuaConsoleBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowLuaConsoleWindow, &mEditBox));

	mTexturesBtn = sfg::Button::Create("Textures");
	mTexturesBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowTexturesWindow, &mEditBox));

	mDecoCreationBtn = sfg::Button::Create("Deco Creation");
	mDecoCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowDecoCreationWindow, &mEditBox));

	mPolyCreationBtn = sfg::Button::Create("Poly Creation");
	mPolyCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowPolyCreationWindow, &mEditBox));

	mBasicBodyCreationBtn = sfg::Button::Create("BasicBody Creation");
	mBasicBodyCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowBasicBodyCreationWindow, &mEditBox));

	// Ajoute les �l�ments � la fen�tre
	AddToVBox(mOpenLevelBtn);
	AddToVBox(mOpenLuaConsoleBtn);
	AddToVBox(mTexturesBtn);
	AddToVBox(mDecoCreationBtn);
	AddToVBox(mPolyCreationBtn);
	AddToVBox(mBasicBodyCreationBtn);
}
