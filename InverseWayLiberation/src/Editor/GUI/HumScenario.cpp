#include "HumScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"

// Ctor
HumScenario::HumScenario(EditBox &editBox)
        : Scenario(editBox, "Hum"),
          mEntityMgr(EntityManager::GetInstance()), mResourceMgr(ResourceManager::GetInstance()),
          mSelection(nullptr) {
    // Initialise le pas
    mPosStepSaveValue = 1.f;

    // Rempli la fenêtre
    Fill();
    mApply = true;
}

// Gestion de la sélection
void HumScenario::Select(Hum *selection) {
    myAssert(selection, "BasicBody passé invalide.");

    mSelection = selection;

    OnRefresh();
}

void HumScenario::Unselect() {
    mSelection = nullptr;
}

// Actualisation
void HumScenario::Update() {
    if (!mSelection) return;

    // Met à jour les valeurs
    mPosX->SetText(Parser::floatToString(mSelection->GetPosition().x, 4));
    mPosY->SetText(Parser::floatToString(mSelection->GetPosition().y, 4));

    // Couleur
    mColorR->SetValue(mSelection->GetTrunkColor().r);
    mColorG->SetValue(mSelection->GetTrunkColor().g);
    mColorB->SetValue(mSelection->GetTrunkColor().b);

    // Gère le Layer
    mLayer->SetValue(static_cast<float>(mSelection->GetLayer()));
}

// Construit la fenêtre et les éléments
void HumScenario::Fill() {
    // Position et rotation
    mPosTable = sfg::Table::Create();
    mPosLabel = sfg::Label::Create(L"Pos :");
    mPosX = sfg::Entry::Create();
    mPosX->SetRequisition(sf::Vector2f(60.f, 0.f));
    mPosY = sfg::Entry::Create();
    mPosY->SetRequisition(sf::Vector2f(60.f, 0.f));
    mPosButton = sfg::Button::Create(L"X");
    mPosButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnChangePosition, this));
    mPosStep = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
    mPosStep->SetValue(mPosStepSaveValue);
    mPosStep->SetDigits(1);
    mPosStep->SetRequisition(sf::Vector2f(40.f, 0.f));
    mPosXp = sfg::Button::Create(L"+");
    mPosXp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnChangePosXp, this));
    mPosXm = sfg::Button::Create(L"-");
    mPosXm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnChangePosXm, this));
    mPosYp = sfg::Button::Create(L"+");
    mPosYp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnChangePosYp, this));
    mPosYm = sfg::Button::Create(L"-");
    mPosYm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnChangePosYm, this));
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
    mColorLabel = sfg::Label::Create(L"Couleur : ");
    mColorR = sfg::SpinButton::Create(0.f, 255.f, 1.f);
    mColorR->SetDigits(0);
    mColorR->SetValue(35);
    mColorR->SetRequisition(sf::Vector2f(60.f, 0.f));
    mColorR->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&HumScenario::OnChangeColor, this));
    mColorG = sfg::SpinButton::Create(0.f, 255.f, 1.f);
    mColorG->SetDigits(0);
    mColorG->SetValue(35);
    mColorG->SetRequisition(sf::Vector2f(60.f, 0.f));
    mColorG->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&HumScenario::OnChangeLayer, this));
    mColorB = sfg::SpinButton::Create(0.f, 255.f, 1.f);
    mColorB->SetDigits(0);
    mColorB->SetValue(35);
    mColorB->SetRequisition(sf::Vector2f(60.f, 0.f));
    mColorB->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&HumScenario::OnChangeLayer, this));
    mColorHBox->PackEnd(mColorLabel, false);
    mColorHBox->PackEnd(mColorR);
    mColorHBox->PackEnd(mColorG);
    mColorHBox->PackEnd(mColorB);

    // Layer
    mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mLayerLabel = sfg::Label::Create(L"Layer : ");
    mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
    mLayer->SetValue(1);
    mLayer->SetDigits(0);
    mLayer->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&HumScenario::OnChangeLayer, this));
    mLayerHBox->PackEnd(mLayerLabel, false);
    mLayerHBox->PackEnd(mLayer);

    // Bouton Détruire & ClearForces
    mButtonsHBox1 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mDestroy = sfg::Button::Create(L"Détruire");
    mDestroy->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnDestroy, this));
    mClearForces = sfg::Button::Create(L"ClearForces");
    mClearForces->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnClearForces, this));
    mButtonsHBox1->PackEnd(mDestroy);
    mButtonsHBox1->PackEnd(mClearForces);

    // Bouton d'actualisation
    mButtonsHBox2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mRefresh = sfg::Button::Create(L"Actualiser");
    mRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&HumScenario::OnRefresh, this));
    mButtonsHBox2->PackEnd(mRefresh);

    // Ajoute les éléments à la fenêtre
    AddToVBox(mPosTable);
    AddToVBox(mColorHBox);
    AddToVBox(mLayerHBox);
    AddToVBox(mButtonsHBox1);
    AddToVBox(mButtonsHBox2);
}

// Callbacks
void HumScenario::OnChangePosition() {
    if (!mApply || !mSelection) return;

    // Change sa position et sa rotation
    mSelection->SetTransform(b2Vec2(Parser::stringToFloat(mPosX->GetText().toAnsiString()),
                                    Parser::stringToFloat(mPosY->GetText().toAnsiString())));
}

void HumScenario::OnChangePosXp() {
    if (!mApply || !mSelection) return;

    // Change sa position
    mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(-mPosStep->GetValue(), 0.f));

    // Actualise
    OnRefresh();
}

void HumScenario::OnChangePosXm() {
    if (!mApply || !mSelection) return;

    // Change sa position
    mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(mPosStep->GetValue(), 0.f));

    // Actualise
    OnRefresh();
}

void HumScenario::OnChangePosYp() {
    if (!mApply || !mSelection) return;

    // Change sa position
    mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(0.f, -mPosStep->GetValue()));

    // Actualise
    OnRefresh();
}

void HumScenario::OnChangePosYm() {
    if (!mApply || !mSelection) return;

    // Change sa position
    mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(0.f, mPosStep->GetValue()));

    // Actualise
    OnRefresh();
}

void HumScenario::OnChangePosRp() {
    if (!mApply || !mSelection) return;

    // Change sa rotation
    mSelection->SetTransform(mSelection->GetPosition());

    // Actualise
    OnRefresh();
}

void HumScenario::OnChangePosRm() {
    if (!mApply || !mSelection) return;

    // Change sa position
    mSelection->SetTransform(mSelection->GetPosition());

    // Actualise
    OnRefresh();
}

void HumScenario::OnChangeColor() {
    if (!mApply || !mSelection) return;

    mSelection->SetTrunkColor(sf::Color((sf::Uint8) mColorR->GetValue(),
                                        (sf::Uint8) mColorG->GetValue(),
                                        (sf::Uint8) mColorB->GetValue()
    ));
}

void HumScenario::OnChangeLayer() {
    if (!mApply || !mSelection) return;

    mSelection->SetLayer(static_cast<int>(mLayer->GetValue()));
}

void HumScenario::OnClearForces() {
    if (!mApply || !mSelection) return;

    // Obtient le BasicBody
    myAssert(mSelection->GetBody(), "Aucun b2Body associé au BaseBody.");
    mSelection->GetBody()->SetLinearVelocity(b2Vec2_zero);
    mSelection->GetBody()->SetAngularVelocity(0.f);
}

void HumScenario::OnDestroy() {
    if (!mApply || !mSelection) return;

    // Détruit le BaseBody
    mEntityMgr.DestroyEntity(mSelection);
    Unselect();
    mEditBox.ScheduleUnselection();
}
