#include "stdafx.h"
#include "PointLightCreationWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/PointLight.h"
#include "../../Entities/EntityFactory.h"

// Ctor
PointLightCreationWindow::PointLightCreationWindow()
	: Window("PointLightCreation"),
	mEntityMgr(EntityManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()),
	mIsInAddMode(false)
{
	// Rempli la fen�tre
	Fill();
	mApply = true;
}

// Gestion de l'ajout
void PointLightCreationWindow::Add(b2Vec2 pos)
{
	if (!mApply) return;

	// Sort du mode cr�ation si la fen�tre est cach�e
	if (!this->IsVisible() || !mIsInAddMode)
	{
		OnToggleMode();
		return;
	}

	// R�cup�re la couleur
	sf::Color c((sf::Uint8) mColorR->GetValue(), (sf::Uint8) mColorG->GetValue(), (sf::Uint8) mColorB->GetValue());

	// Cr�e la d�co
	EntityFactory::CreatePointLight(pos, (int) mRadius->GetValue(), c, (int) mLayer->GetValue());

	OnRefresh();
}

// Actualisation
void PointLightCreationWindow::Update()
{
}

// Accesseurs
bool PointLightCreationWindow::IsInAddMode()
{
	return mIsInAddMode;
}

// Construit la fen�tre et les �l�ments
void PointLightCreationWindow::Fill()
{
	// Cr�e le Layout
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// El�ments
	mMode = sfg::Label::Create("Mode : Attente");
	mHelpLabel = sfg::Label::Create("Cr�er d�co : Ctrl + clic gauche");

	// Couleur
	mColorHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mColorLabel = sfg::Label::Create("Couleur : ");
	mColorR = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorR->SetDigits(0);
	mColorR->SetValue(100);
	mColorR->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorG = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorG->SetDigits(0);
	mColorG->SetValue(100);
	mColorG->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorB = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorB->SetDigits(0);
	mColorB->SetValue(100);
	mColorB->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorHBox->PackEnd(mColorLabel, false);
	mColorHBox->PackEnd(mColorR);
	mColorHBox->PackEnd(mColorG);
	mColorHBox->PackEnd(mColorB);

	// Rayon
	mRadiusHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mRadiusLabel = sfg::Label::Create("Rayon : ");
	mRadius = sfg::SpinButton::Create(0.f, 1000.f, 20.f);
	mRadius->SetValue(200.f);
	mRadius->SetDigits(0);
	mRadiusHBox->PackEnd(mRadiusLabel, false);
	mRadiusHBox->PackEnd(mRadius);

	// Layer
	mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mLayerLabel = sfg::Label::Create("Layer : ");
	mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
	mLayer->SetValue(5);
	mLayer->SetDigits(0);
	mLayerHBox->PackEnd(mLayerLabel, false);
	mLayerHBox->PackEnd(mLayer);

	// Boutons
	mToggleModeBtn = sfg::Button::Create("Entrer mode Cr�ation");
	mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mRefreshBtn = sfg::Button::Create("Actualiser");
	mCloseBtn = sfg::Button::Create("Fermer");
	mButtonsHBox->PackEnd(mRefreshBtn);
	mButtonsHBox->PackEnd(mCloseBtn);

	// Signaux
	mToggleModeBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightCreationWindow::OnToggleMode, this));
	mRefreshBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightCreationWindow::OnRefresh, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightCreationWindow::OnClose, this));

	// Ajoute les �l�ments � la fen�tre
	mVBox->PackEnd(mMode);
	mVBox->PackEnd(mHelpLabel);
	mVBox->PackEnd(mColorHBox);
	mVBox->PackEnd(mRadiusHBox);
	mVBox->PackEnd(mLayerHBox);
	mVBox->PackEnd(mToggleModeBtn);
	mVBox->PackEnd(mButtonsHBox);

	// Ajoute la mVBox � la fen�tre
	AddToWindow(mVBox);
}

// Callbacks
void PointLightCreationWindow::OnToggleMode()
{
	if (!mApply) return;

	mIsInAddMode = !mIsInAddMode;

	if (mIsInAddMode)
	{
		mMode->SetText("Mode : Cr�ation");
		mToggleModeBtn->SetLabel("Sortir mode Cr�ation");
	}
	else
	{
		mMode->SetText("Mode : Attente");
		mToggleModeBtn->SetLabel("Entrer mode Cr�ation");
	}
}
