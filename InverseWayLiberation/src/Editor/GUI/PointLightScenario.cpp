#include "stdafx.h"
#include "DecoScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Graphics/LightEngine.h"

// Ctor
PointLightScenario::PointLightScenario(EditBox &editBox)
	: Scenario(editBox, "PointLight"),
	mEntityMgr(EntityManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()),
	mSelection(nullptr)
{
	// Initialise le pas
	mPosStepSaveValue = 1.f;

	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de la sélection
void PointLightScenario::Select(PointLight *selection)
{
	myAssert(selection, "BasicBody passé invalide.");

	mSelection = selection;

	OnRefresh();
}
void PointLightScenario::Unselect()
{
	mSelection = nullptr;
}

// Actualisation
void PointLightScenario::Update()
{
	if (!mSelection) return;

	// Met à jour les valeurs
	mPosX->SetText(Parser::floatToString(mSelection->GetPosition().x, 4));
	mPosY->SetText(Parser::floatToString(mSelection->GetPosition().y, 4));

	// Gère le Layer
	mLayer->SetValue(static_cast<float>(mSelection->GetLayer()));

	// Met à jour le rayon
	mRadius->SetValue(static_cast<float>(mSelection->GetLightRadius()));

	// Met à jour la couleur
	mColorR->SetValue(static_cast<float>(mSelection->GetLightColor().r));
	mColorG->SetValue(static_cast<float>(mSelection->GetLightColor().g));
	mColorB->SetValue(static_cast<float>(mSelection->GetLightColor().b));
}

// Construit la fenêtre et les éléments
void PointLightScenario::Fill()
{
	// Position
	mPosTable = sfg::Table::Create();
	mPosLabel = sfg::Label::Create("Pos :");
	mPosX = sfg::Entry::Create();
	mPosX->SetRequisition(sf::Vector2f(60.f, 0.f));
	mPosY = sfg::Entry::Create();
	mPosY->SetRequisition(sf::Vector2f(60.f, 0.f));
	mPosButton = sfg::Button::Create("X");
	mPosButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightScenario::OnChangePosition, this));
	mPosStep = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
	mPosStep->SetValue(mPosStepSaveValue);
	mPosStep->SetDigits(1);
	mPosStep->SetRequisition(sf::Vector2f(40.f, 0.f));
	mPosXp = sfg::Button::Create("+");
	mPosXp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightScenario::OnChangePosXp, this));
	mPosXm = sfg::Button::Create("-");
	mPosXm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightScenario::OnChangePosXm, this));
	mPosYp = sfg::Button::Create("+");
	mPosYp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightScenario::OnChangePosYp, this));
	mPosYm = sfg::Button::Create("-");
	mPosYm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightScenario::OnChangePosYm, this));
	mPosTable->Attach(mPosLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mPosTable->Attach(mPosX, sf::Rect<sf::Uint32>(2, 1, 4, 1));
	mPosTable->Attach(mPosY, sf::Rect<sf::Uint32>(6, 1, 4, 1));
	mPosTable->Attach(mPosButton, sf::Rect<sf::Uint32>(14, 1, 1, 1));
	mPosTable->Attach(mPosStep, sf::Rect<sf::Uint32>(1, 2, 1, 1));
	mPosTable->Attach(mPosXm, sf::Rect<sf::Uint32>(2, 2, 2, 1));
	mPosTable->Attach(mPosXp, sf::Rect<sf::Uint32>(4, 2, 2, 1));
	mPosTable->Attach(mPosYp, sf::Rect<sf::Uint32>(6, 2, 2, 1));
	mPosTable->Attach(mPosYm, sf::Rect<sf::Uint32>(8, 2, 2, 1));

	// Couleur
	mColorHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mColorLabel = sfg::Label::Create("Couleur : ");
	mColorR = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorR->SetDigits(0);
	mColorR->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorR->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&PointLightScenario::OnChangeColorR, this));
	mColorG = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorG->SetDigits(0);
	mColorG->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorG->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&PointLightScenario::OnChangeColorG, this));
	mColorB = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mColorB->SetDigits(0);
	mColorB->SetRequisition(sf::Vector2f(60.f, 0.f));
	mColorB->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&PointLightScenario::OnChangeColorB, this));
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
	mRadius->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&PointLightScenario::OnChangeRadius, this));
	mRadiusHBox->PackEnd(mRadiusLabel, false);
	mRadiusHBox->PackEnd(mRadius);

	// Layer
	mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mLayerLabel = sfg::Label::Create("Layer : ");
	mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
	mLayer->SetValue(5);
	mLayer->SetDigits(0);
	mLayer->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&PointLightScenario::OnChangeLayer, this));
	mLayerHBox->PackEnd(mLayerLabel, false);
	mLayerHBox->PackEnd(mLayer);

	// Boutons
	mRefresh = sfg::Button::Create("Actualiser");
	mRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightScenario::OnRefresh, this));
	mDestroy = sfg::Button::Create("Détruire");
	mDestroy->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PointLightScenario::OnDestroy, this));

	// Ajoute les éléments à la fenêtre
	AddToVBox(mPosTable);
	AddToVBox(mColorHBox);
	AddToVBox(mRadiusHBox);
	AddToVBox(mLayerHBox);
	AddToVBox(mRefresh);
	AddToVBox(mDestroy);
}

// Callbacks
void PointLightScenario::OnChangePosition()
{
	if (!mApply || !mSelection) return;

	// Change sa position et sa rotation
	mSelection->SetPosition(b2Vec2(Parser::stringToFloat(mPosX->GetText()), Parser::stringToFloat(mPosY->GetText())));
	OnRefresh();
}
void PointLightScenario::OnChangeLayer()
{
	if (!mApply || !mSelection) return;

	mSelection->SetLayer(static_cast<int>(mLayer->GetValue()));
	OnRefresh();
}
void PointLightScenario::OnChangePosXp()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->Move(b2Vec2(mPosStep->GetValue(), 0.f));
	OnRefresh();
}
void PointLightScenario::OnChangePosXm()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->Move(b2Vec2(-mPosStep->GetValue(), 0.f));
	OnRefresh();
}
void PointLightScenario::OnChangePosYp()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->Move(b2Vec2(0.f, mPosStep->GetValue()));
	OnRefresh();
}
void PointLightScenario::OnChangePosYm()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->Move(b2Vec2(0.f, -mPosStep->GetValue()));
	OnRefresh();
}
void PointLightScenario::OnChangeRadius()
{
	if (!mApply || !mSelection) return;

	// Change son rayon
	mSelection->SetLightRadius(static_cast<unsigned int>(mRadius->GetValue()));
	OnRefresh();
}
void PointLightScenario::OnChangeColorR()
{
	if (!mApply || !mSelection) return;

	// Change sa couleur
	sf::Color c = mSelection->GetLightColor();
	c.r = (sf::Uint8) mColorR->GetValue();
	mSelection->SetLightColor(c);
	OnRefresh();
}
void PointLightScenario::OnChangeColorG()
{
	if (!mApply || !mSelection) return;

	// Change sa couleur
	sf::Color c = mSelection->GetLightColor();
	c.g = (sf::Uint8) mColorG->GetValue();
	mSelection->SetLightColor(c);
	OnRefresh();
}
void PointLightScenario::OnChangeColorB()
{
	if (!mApply || !mSelection) return;

	// Change sa couleur
	sf::Color c = mSelection->GetLightColor();
	c.b = (sf::Uint8) mColorB->GetValue();
	mSelection->SetLightColor(c);
	OnRefresh();
}
void PointLightScenario::OnDestroy()
{
	if (!mApply || !mSelection) return;

	// Détruit le BasicBody
	mEntityMgr.DestroyEntity(mSelection);
	Unselect();
	mEditBox.ScheduleUnselection();
}
