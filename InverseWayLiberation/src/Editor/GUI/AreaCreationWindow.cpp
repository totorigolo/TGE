#include "AreaCreationWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/EntityFactory.h"
#include "../../Tools/utils.h"

#include <SFGUI/Separator.hpp>

// Ctor
AreaCreationWindow::AreaCreationWindow()
        : Window("AreaCreation"),
          mIsInAddMode(false),
          mEntityMgr(EntityManager::GetInstance()),
          mPhysicMgr(PhysicManager::GetInstance()),
          mResourceMgr(ResourceManager::GetInstance()),
          mTriggersMgr(TriggersManager::GetInstance())
{
    // Rempli la fenêtre
    Fill();
    mApply = true;
}

// Gestion de l'ajout
void AreaCreationWindow::Add(b2Vec2 pos) {
    if (!mApply || mScript->GetSelectedItem() == sfg::ComboBox::NONE) return;

    // Sort du mode création si la fenêtre est cachée
    if (!this->IsVisible() || !mIsInAddMode) {
        OnToggleMode();
        return;
    }

    // Récupère les dimensions
    float width = Parser::stringToFloat(mWidth->GetText(), -1);
    float height = Parser::stringToFloat(mHeight->GetText(), -1);

    // Vérifie que toutes les données nécessaires sont fournies
    if (width < 0 || height < 0 || mScript->GetSelectedText().isEmpty()) return;

    // Crée l'AABB
    b2AABB box;
    box.upperBound.x = pos.x + width;
    box.upperBound.y = pos.y;
    box.lowerBound.x = pos.x;
    box.lowerBound.y = pos.y - height;

    // Crée l'Area
    TriggersManager::GetInstance().CreateArea(box, mScript->GetSelectedText(), mOnce->IsActive());

    OnRefresh();
}

// Actualisation
void AreaCreationWindow::Update() {
    if (!mScript.get()) return;

    // Mets à jour la liste des scripts
    auto currentScript = mScript->GetSelectedText();
    mScript->Clear();
    for (auto &actionEntry : mTriggersMgr.GetActionMap()) {
        mScript->AppendItem(actionEntry.first);
    }
    for (int i = mScript->GetItemCount(); i > 0; --i) {
        if (mScript->GetItem(i) == currentScript) {
            mScript->SelectItem(i);
        }
    }
}

// Accesseurs
bool AreaCreationWindow::IsInAddMode() {
    return mIsInAddMode;
}

// Construit la fenêtre et les éléments
void AreaCreationWindow::Fill() {
    // Crée le Layout
    mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    // Eléments
    mMode = sfg::Label::Create(L"Mode : Attente");
    mHelpLabel = sfg::Label::Create(L"Créer Area : Ctrl + clic gauche");

    // Positions
    mDimTable = sfg::Table::Create();
    mWidthLabel = sfg::Label::Create("Largeur");
    mHeightLabel = sfg::Label::Create("Hauteur");
    mWidth = sfg::Entry::Create();
    mWidth->SetRequisition(sf::Vector2f(60.f, 0.f));
    mHeight = sfg::Entry::Create();
    mHeight->SetRequisition(sf::Vector2f(60.f, 0.f));
    mDimTable->Attach(mWidthLabel, sf::Rect<sf::Uint32>(1, 1, 3, 1));
    mDimTable->Attach(mWidth, sf::Rect<sf::Uint32>(4, 1, 3, 1));
    mDimTable->Attach(mHeightLabel, sf::Rect<sf::Uint32>(1, 2, 3, 1));
    mDimTable->Attach(mHeight, sf::Rect<sf::Uint32>(4, 2, 3, 1));

    // Script
    mScriptHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mScriptLabel = sfg::Label::Create(L"Script :");
    mScript = sfg::ComboBox::Create();
    mScriptHBox->PackEnd(mScriptLabel, false);
    mScriptHBox->PackEnd(mScript);
    mOnce = sfg::CheckButton::Create(L"One shot ?");

    // Boutons
    mToggleModeBtn = sfg::Button::Create(L"Entrer mode Création");
    mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mRefreshBtn = sfg::Button::Create(L"Actualiser");
    mCloseBtn = sfg::Button::Create(L"Fermer");
    mButtonsHBox->PackEnd(mRefreshBtn);
    mButtonsHBox->PackEnd(mCloseBtn);

    // Signaux
    mToggleModeBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&AreaCreationWindow::OnToggleMode, this));
    mRefreshBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&AreaCreationWindow::OnRefresh, this));
    mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&AreaCreationWindow::OnClose, this));

    // Ajoute les éléments à la fenêtre
    mVBox->Pack(mDimTable);
    mVBox->Pack(mScriptHBox);
    mVBox->Pack(sfg::Separator::Create());
    mVBox->Pack(mOnce);
    mVBox->Pack(sfg::Separator::Create());
    mVBox->Pack(mToggleModeBtn);
    mVBox->Pack(mButtonsHBox);

    // Ajoute la mVBox à la fenêtre
    AddToWindow(mVBox);
}

// Callbacks
void AreaCreationWindow::OnToggleMode() {
    if (!mApply) return;

    mIsInAddMode = !mIsInAddMode;

    if (mIsInAddMode) {
        mMode->SetText(L"Mode : Création");
        mToggleModeBtn->SetLabel(L"Sortir mode Création");
    } else {
        mMode->SetText(L"Mode : Attente");
        mToggleModeBtn->SetLabel(L"Entrer mode Création");
    }
}
