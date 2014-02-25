#include "stdafx.h"
#include "LevelWindow.h"
#include "../../App/InputManager.h"

// Ctor
LevelWindow::LevelWindow()
	: Window("Level"),
	mLevelMgr(LevelManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance())
{
	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Actualisation
void LevelWindow::Update()
{
	// Gravité
	mGravityX->SetValue(mPhysicMgr.GetGravity().x);
	mGravityY->SetValue(mPhysicMgr.GetGravity().y);

	// Couleur du fond
	mBckgColorR->SetValue(mLevelMgr.GetBckgColor().r);
	mBckgColorG->SetValue(mLevelMgr.GetBckgColor().g);
	mBckgColorB->SetValue(mLevelMgr.GetBckgColor().b);

	// Origine de la vue
	mOriginViewX->SetValue(mLevelMgr.GetDefaultCenter().x * mPhysicMgr.GetMPP());
	mOriginViewY->SetValue(mLevelMgr.GetDefaultCenter().y * mPhysicMgr.GetMPP());

	// Zoom par défaut
	mDefaultZoom->SetValue(mLevelMgr.GetDefaultZoom());
}

// Construit la fenêtre et les éléments
void LevelWindow::Fill()
{
	// Widgets
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mGravityBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mGravityLabel = sfg::Label::Create("Gravité : ");
	mGravityX = sfg::SpinButton::Create(-2000.f, 2000.f, 0.2f);
	mGravityX->SetDigits(2);
	mGravityY = sfg::SpinButton::Create(-2000.f, 2000.f, 0.2f);
	mGravityY->SetDigits(2);
	mGravityBox->PackEnd(mGravityLabel, false);
	mGravityBox->PackEnd(mGravityX);
	mGravityBox->PackEnd(mGravityY);
	mBckgColorBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mBckgColorLabel = sfg::Label::Create("BckgC : ");
	mBckgColorR = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mBckgColorR->SetDigits(0);
	mBckgColorR->SetRequisition(sf::Vector2f(60.f, 0.f));
	mBckgColorG = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mBckgColorG->SetDigits(0);
	mBckgColorG->SetRequisition(sf::Vector2f(60.f, 0.f));
	mBckgColorB = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mBckgColorB->SetDigits(0);
	mBckgColorB->SetRequisition(sf::Vector2f(60.f, 0.f));
	mBckgColorBox->PackEnd(mBckgColorLabel, false);
	mBckgColorBox->PackEnd(mBckgColorR);
	mBckgColorBox->PackEnd(mBckgColorG);
	mBckgColorBox->PackEnd(mBckgColorB);
	mOriginViewBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mOriginViewLabel = sfg::Label::Create("Origine : ");
	mOriginViewX = sfg::SpinButton::Create(-1000000.f, 1000000.f, 1.f);
	mOriginViewX->SetDigits(3);
	mOriginViewY = sfg::SpinButton::Create(-1000000.f, 1000000.f, 1.f);
	mOriginViewY->SetDigits(3);
	mOriginViewCurrentBtn = sfg::Button::Create("C");
	mOriginViewBox->PackEnd(mOriginViewLabel, false);
	mOriginViewBox->PackEnd(mOriginViewX);
	mOriginViewBox->PackEnd(mOriginViewY);
	mOriginViewBox->PackEnd(mOriginViewCurrentBtn, false);
	mDefaultZoomBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mDefaultZoomLabel = sfg::Label::Create("Déf Zoom : ");
	mDefaultZoom = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
	mDefaultZoom->SetDigits(3);
	mDefaultZoomCurrentBtn = sfg::Button::Create("C");
	mDefaultZoomBox->PackEnd(mDefaultZoomLabel, false);
	mDefaultZoomBox->PackEnd(mDefaultZoom);
	mDefaultZoomBox->PackEnd(mDefaultZoomCurrentBtn, false);
	mButtonBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mRefreshBtn = sfg::Button::Create("Actualiser");
	mCloseBtn = sfg::Button::Create("Fermer");
	mButtonBox->PackEnd(mRefreshBtn);
	mButtonBox->PackEnd(mCloseBtn);

	// Connecte les signaux
	mGravityX->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeGravityX, this));
	mGravityY->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeGravityY, this));
	mBckgColorR->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeBckgColorR, this));
	mBckgColorG->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeBckgColorG, this));
	mBckgColorB->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeBckgColorB, this));
	mOriginViewX->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeOriginViewX, this));
	mOriginViewY->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeOriginViewY, this));
	mOriginViewCurrentBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&LevelWindow::OnChangeOriginViewCurrent, this));
	mDefaultZoom->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&LevelWindow::OnChangeDefaultZoom, this));
	mDefaultZoomCurrentBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&LevelWindow::OnChangeDefaultZoomCurrent, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&LevelWindow::OnClose, this));

	// Ajoute les éléments à la fenêtre
	mVBox->PackEnd(mGravityBox);
	mVBox->PackEnd(mBckgColorBox);
	mVBox->PackEnd(mOriginViewBox);
	mVBox->PackEnd(mDefaultZoomBox);
	mVBox->PackEnd(mCloseBtn);

	// Ajoute la mLevelVBox à la fenêtre
	AddToWindow(mVBox);
}

// Callbacks
void LevelWindow::OnChangeGravityX()
{
	if (!mApply) return;

	mPhysicMgr.SetGravity(b2Vec2(mGravityX->GetValue(), mPhysicMgr.GetGravity().y));
}
void LevelWindow::OnChangeGravityY()
{
	if (!mApply) return;

	mPhysicMgr.SetGravity(b2Vec2(mPhysicMgr.GetGravity().x, mGravityY->GetValue()));
}
void LevelWindow::OnChangeBckgColorR()
{
	if (!mApply) return;

	sf::Color c = mLevelMgr.GetBckgColor();
	c.r = static_cast<sf::Uint8>(mBckgColorR->GetValue());
	mLevelMgr.SetBckgColor(c);
}
void LevelWindow::OnChangeBckgColorG()
{
	if (!mApply) return;

	sf::Color c = mLevelMgr.GetBckgColor();
	c.g = static_cast<sf::Uint8>(mBckgColorG->GetValue());
	mLevelMgr.SetBckgColor(c);
}
void LevelWindow::OnChangeBckgColorB()
{
	if (!mApply) return;

	sf::Color c = mLevelMgr.GetBckgColor();
	c.b = static_cast<sf::Uint8>(mBckgColorB->GetValue());
	mLevelMgr.SetBckgColor(c);
}
void LevelWindow::OnChangeOriginViewX()
{
	if (!mApply) return;

	sf::Vector2f c = mLevelMgr.GetDefaultCenter();
	c.x = mOriginViewX->GetValue() * mPhysicMgr.GetPPM();
	mLevelMgr.SetDefaultCenter(c);
}
void LevelWindow::OnChangeOriginViewY()
{
	if (!mApply) return;

	sf::Vector2f c = mLevelMgr.GetDefaultCenter();
	c.y = mOriginViewY->GetValue() * mPhysicMgr.GetPPM();
	mLevelMgr.SetDefaultCenter(c);
}
void LevelWindow::OnChangeOriginViewCurrent()
{
	if (!mApply) return;

	mLevelMgr.SetDefaultCenter(InputManager::GetInstance().GetCurrentCenter() * mPhysicMgr.GetMPP());

	mOriginViewX->SetValue(mLevelMgr.GetDefaultCenter().x);
	mOriginViewY->SetValue(mLevelMgr.GetDefaultCenter().y);
}
void LevelWindow::OnChangeDefaultZoom()
{
	if (!mApply) return;

	mLevelMgr.SetDefaultZoom(mDefaultZoom->GetValue());
}
void LevelWindow::OnChangeDefaultZoomCurrent()
{
	if (!mApply) return;

	mDefaultZoom->SetValue(InputManager::GetInstance().GetCurrentZoom());
}
