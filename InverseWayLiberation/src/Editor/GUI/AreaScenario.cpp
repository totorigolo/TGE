#include "AreaScenario.h"
#include "../EditBox.h"

#include <SFGUI/Separator.hpp>

// Ctor
AreaScenario::AreaScenario(EditBox &editBox)
        : Scenario(editBox, "Area"),
          mEntityMgr(EntityManager::GetInstance()),
          mPhysicMgr(PhysicManager::GetInstance()),
          mResourceMgr(ResourceManager::GetInstance()),
          mTriggersMgr(TriggersManager::GetInstance()),
          mSelection(nullptr) {
    // Rempli la fenêtre
    Fill();
    mApply = true;
}

// Gestion de la sélection
void AreaScenario::Select(ScriptArea *selection) {
    myAssert(selection, "ScriptArea passé invalide.");

    mSelection = selection;

    OnRefresh();
}

void AreaScenario::Unselect() {
    mSelection = nullptr;
}

// Actualisation
void AreaScenario::Update() {
    if (!mSelection) return;

    // Met à jour les valeurs
    mPosRight->SetText(Parser::floatToString(mSelection->GetAABB().upperBound.x, 4));
    mPosTop->SetText(Parser::floatToString(mSelection->GetAABB().upperBound.y, 4));
    mPosLeft->SetText(Parser::floatToString(mSelection->GetAABB().lowerBound.x, 4));
    mPosBottom->SetText(Parser::floatToString(mSelection->GetAABB().lowerBound.y, 4));

    // Met à jour le One Shot
    mOnce->SetActive(mSelection->IsOnce());

    // Met à jour la liste des scripts, et le script actuel
    for (int i = mScript->GetItemCount(); i > 0; --i) {
        mScript->RemoveItem(i - 1);
    }
    int index = 0;
    for (auto &actionEntry : mTriggersMgr.GetActionMap()) {
        mScript->AppendItem(actionEntry.first);

        if (actionEntry.first == mSelection->GetAction())
            mScript->SelectItem(index);
        ++index;
    }
}

// Construit la fenêtre et les éléments
void AreaScenario::Fill() {
    // Positions
    mPosTable = sfg::Table::Create();
    mPosTop = sfg::Entry::Create();
    mPosTop->SetRequisition(sf::Vector2f(60.f, 0.f));
    mPosLeft = sfg::Entry::Create();
    mPosLeft->SetRequisition(sf::Vector2f(60.f, 0.f));
    mPosBottom = sfg::Entry::Create();
    mPosBottom->SetRequisition(sf::Vector2f(60.f, 0.f));
    mPosRight = sfg::Entry::Create();
    mPosRight->SetRequisition(sf::Vector2f(60.f, 0.f));
    mPosButton = sfg::Button::Create(L"Changer position");
    mPosButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&AreaScenario::OnChangePosition, this));
    mPosTable->Attach(mPosTop, sf::Rect<sf::Uint32>(3, 1, 3, 1));
    mPosTable->Attach(mPosLeft, sf::Rect<sf::Uint32>(1, 2, 3, 1));
    mPosTable->Attach(mPosRight, sf::Rect<sf::Uint32>(5, 2, 3, 1));
    mPosTable->Attach(mPosBottom, sf::Rect<sf::Uint32>(3, 3, 3, 1));
    mPosTable->Attach(mPosButton, sf::Rect<sf::Uint32>(1, 4, 8, 1));

    // Script
    mScriptHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mScriptLabel = sfg::Label::Create(L"Script :");
    mScript = sfg::ComboBox::Create();
    mScript->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&AreaScenario::OnChangeScript, this));
    mScriptHBox->PackEnd(mScriptLabel, false);
    mScriptHBox->PackEnd(mScript);
    mOnce = sfg::CheckButton::Create(L"One shot ?");
    mOnce->GetSignal(sfg::Button::OnStateChange).Connect(std::bind(&AreaScenario::OnChangeOnce, this));

    // Boutons
    mRefresh = sfg::Button::Create(L"Actualiser");
    mRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&AreaScenario::OnRefresh, this));
    mDestroy = sfg::Button::Create(L"Détruire");
    mDestroy->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&AreaScenario::OnDestroy, this));

    // Ajoute les éléments à la fenêtre
    AddToVBox(mPosTable);
    AddToVBox(mScriptHBox);
    AddToVBox(mOnce);
    AddToVBox(mRefresh);
    AddToVBox(mDestroy);
}

// Callbacks
void AreaScenario::OnChangePosition() {
    if (!mApply || !mSelection) return;

    // Récupère les valeurs
    float top = Parser::stringToFloat(mPosTop->GetText().toAnsiString());
    float left = Parser::stringToFloat(mPosLeft->GetText().toAnsiString());
    float bottom = Parser::stringToFloat(mPosBottom->GetText().toAnsiString());
    float right = Parser::stringToFloat(mPosRight->GetText().toAnsiString());

    // Change la position de l'area
    mSelection->GetAABB().upperBound.x = right;
    mSelection->GetAABB().upperBound.y = top;
    mSelection->GetAABB().lowerBound.x = left;
    mSelection->GetAABB().lowerBound.y = bottom;
    OnRefresh();
}

void AreaScenario::OnChangeScript() {
    if (!mApply || !mSelection) return;

    mSelection->SetAction(mScript->GetSelectedText());

    OnRefresh();
}

void AreaScenario::OnChangeOnce() {
    if (!mApply || !mSelection) return;

    mSelection->SetOnce(mOnce->IsActive());

    OnRefresh();
}

void AreaScenario::OnDestroy() {
    if (!mApply || !mSelection) return;

    // Détruit le BasicBody
    mTriggersMgr.DestroyArea(mSelection);
    Unselect();
    mEditBox.ScheduleUnselection();
}
