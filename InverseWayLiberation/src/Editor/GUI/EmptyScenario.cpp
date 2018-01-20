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
	mOpenLevelBtn = sfg::Button::Create(L"Level management");
	mOpenLevelBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowLevelWindow, &mEditBox));

	mOpenScriptConsoleBtn = sfg::Button::Create(L"Script Console");
	mOpenScriptConsoleBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowScriptConsoleWindow, &mEditBox));

	mTexturesBtn = sfg::Button::Create(L"Textures");
	mTexturesBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowTexturesWindow, &mEditBox));

	mAreasBtn = sfg::Button::Create(L"Actions");
	mAreasBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowActionsWindow, &mEditBox));

	mAreaCreationBtn = sfg::Button::Create(L"Area Creation");
	mAreaCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowAreaCreationWindow, &mEditBox));

	mDecoCreationBtn = sfg::Button::Create(L"Deco Creation");
	mDecoCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowDecoCreationWindow, &mEditBox));

	mPolyCreationBtn = sfg::Button::Create(L"Poly Creation");
	mPolyCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowPolyCreationWindow, &mEditBox));

	mBasicBodyCreationBtn = sfg::Button::Create(L"BasicBody Creation");
	mBasicBodyCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowBasicBodyCreationWindow, &mEditBox));

	mPointLightCreationBtn = sfg::Button::Create(L"PointLight Creation");
	mPointLightCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowPointLightCreationWindow, &mEditBox));

	mHumCreationBtn = sfg::Button::Create(L"Hum Creation");
	mHumCreationBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::ShowHumCreationWindow, &mEditBox));

	// Ajoute les éléments à la fenêtre
	AddToVBox(mOpenLevelBtn);
	AddToVBox(mOpenScriptConsoleBtn);
	AddToVBox(mTexturesBtn);
	AddToVBox(mAreasBtn);
	AddToVBox(mAreaCreationBtn);
	AddToVBox(mDecoCreationBtn);
	AddToVBox(mPolyCreationBtn);
	AddToVBox(mBasicBodyCreationBtn);
	AddToVBox(mPointLightCreationBtn);
	AddToVBox(mHumCreationBtn);
}
