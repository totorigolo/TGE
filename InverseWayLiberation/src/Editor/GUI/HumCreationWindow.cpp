#include "HumCreationWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/PolyBody.h"
#include "../../Entities/EntityFactory.h"

// Ctor
HumCreationWindow::HumCreationWindow()
	: Window("HumCreation"),
	mEntityMgr(EntityManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()), mResourceMgr(ResourceManager::GetInstance()),
	mIsInAddMode(false)
{
	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de l'ajout
void HumCreationWindow::Add(b2Vec2 pos)
{
	if (!mApply) return;

	// Sort du mode création si la fenêtre est cachée
	if (!this->IsVisible() || !mIsInAddMode)
	{
		OnToggleMode();
		return;
	}

	// Récupère la couleur
	sf::Color c((sf::Uint8) mColorR->GetValue(), (sf::Uint8) mColorG->GetValue(), (sf::Uint8) mColorB->GetValue());

	// Crée le Body suivant la forme
	EntityFactory::CreateHum(pos, mAge->GetValue(), mStrengh->GetValue(), c, (int) mLayer->GetValue());

	OnRefresh();
}

// Actualisation
void HumCreationWindow::Update()
{
}

// Accesseurs
bool HumCreationWindow::IsInAddMode()
{
	return mIsInAddMode;
}

// Construit la fenêtre et les éléments
void HumCreationWindow::Fill()
{
	// Crée le Layout
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// Eléments
	mMode = sfg::Label::Create("Mode : Attente");
	mHelpLabel = sfg::Label::Create("Créer hum : Ctrl + clic gauche");

	// Age
	mAgeHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mAgeLabel = sfg::Label::Create("Age : ");
	mAge = sfg::SpinButton::Create(0.f, 100.f, 1.f);
	mAge->SetValue(18.f);
	mAge->SetDigits(1);
	mAgeHBox->PackEnd(mAgeLabel, false);
	mAgeHBox->PackEnd(mAge);

	// Force
	mStrenghHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mStrenghLabel = sfg::Label::Create("Force : ");
	mStrengh = sfg::SpinButton::Create(-20.f, 20.f, 1.f);
	mStrengh->SetValue(0.f);
	mStrengh->SetDigits(1);
	mStrenghHBox->PackEnd(mStrenghLabel, false);
	mStrenghHBox->PackEnd(mStrengh);

	// Couleur
	mColorHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mColorLabel = sfg::Label::Create("Couleur : ");
	mColorR = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorR->SetDigits(0);
	mColorR->SetValue(35);
	mColorR->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorG = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorG->SetDigits(0);
	mColorG->SetValue(35);
	mColorG->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorB = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorB->SetDigits(0);
	mColorB->SetValue(35);
	mColorB->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorHBox->PackEnd(mColorLabel, false);
	mColorHBox->PackEnd(mColorR);
	mColorHBox->PackEnd(mColorG);
	mColorHBox->PackEnd(mColorB);

	// Layer
	mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mLayerLabel = sfg::Label::Create("Layer : ");
	mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
	mLayer->SetValue(1);
	mLayer->SetDigits(0);
	mLayerHBox->PackEnd(mLayerLabel, false);
	mLayerHBox->PackEnd(mLayer);

	// Boutons
	mToggleModeBtn = sfg::Button::Create("Entrer mode Création");
	mCloseBtn = sfg::Button::Create("Fermer");

	// Signaux
	mToggleModeBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumCreationWindow::OnToggleMode, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumCreationWindow::OnClose, this));

	// Ajoute les éléments à la fenêtre
	mVBox->PackEnd(mMode);
	mVBox->PackEnd(mHelpLabel);
	mVBox->PackEnd(mAgeHBox);
	mVBox->PackEnd(mStrenghHBox);
	mVBox->PackEnd(mColorHBox);
	mVBox->PackEnd(mLayerHBox);
	mVBox->PackEnd(mToggleModeBtn);
	mVBox->PackEnd(mCloseBtn);

	// Ajoute la mVBox à la fenêtre
	AddToWindow(mVBox);
}

// Callbacks
void HumCreationWindow::OnToggleMode()
{
	if (!mApply) return;

	mIsInAddMode = !mIsInAddMode;

	if (mIsInAddMode)
	{
		mMode->SetText("Mode : Création");
		mToggleModeBtn->SetLabel("Sortir mode Création");
	}
	else
	{
		mMode->SetText("Mode : Attente");
		mToggleModeBtn->SetLabel("Entrer mode Création");
	}
}
