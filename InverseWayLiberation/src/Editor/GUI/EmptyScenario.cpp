#include "EmptyScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"

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

	mTexturesBtn = sfg::Button::Create("Textures");
	mTexturesBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowTexturesWindow, &mEditBox));

	mDecoCreationBtn = sfg::Button::Create("Deco Creation");
	mDecoCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowDecoCreationWindow, &mEditBox));

	mPolyCreationBtn = sfg::Button::Create("Poly Creation");
	mPolyCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowPolyCreationWindow, &mEditBox));

	mBasicBodyCreationBtn = sfg::Button::Create("BasicBody Creation");
	mBasicBodyCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowBasicBodyCreationWindow, &mEditBox));

	mPointLightCreationBtn = sfg::Button::Create("PointLight Creation");
	mPointLightCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowPointLightCreationWindow, &mEditBox));

	mHumCreationBtn = sfg::Button::Create("Hum Creation");
	mHumCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowHumCreationWindow, &mEditBox));

	// Ajoute les éléments à la fenêtre
	AddToVBox(mOpenLevelBtn);
	AddToVBox(mOpenLuaConsoleBtn);
	AddToVBox(mTexturesBtn);
	AddToVBox(mDecoCreationBtn);
	AddToVBox(mPolyCreationBtn);
	AddToVBox(mBasicBodyCreationBtn);
	AddToVBox(mPointLightCreationBtn);
	AddToVBox(mHumCreationBtn);
}
