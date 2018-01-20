#include "ActionsWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/PolyBody.h"
#include "../../Entities/EntityFactory.h"
#include "../../Scripts/TriggersManager.h"
#include "../../Scripts/ScriptAction.h"

// Ctor
ActionsWindow::ActionsWindow()
        : Window("Actions"),
          mEntityMgr(EntityManager::GetInstance()),
          mResourceMgr(ResourceManager::GetInstance()),
          mTriggersMgr(TriggersManager::GetInstance()) {
    // Rempli la fenêtre
    Fill();
    mApply = true;
}

// Actualisation
void ActionsWindow::Update() {
    // Mets à jour la liste de texture
    if (mAction.get()) {
        auto current = mAction->GetSelectedItem();
        for (int i = mAction->GetItemCount(); i > 0; --i)
            mAction->RemoveItem(i - 1);
        for (auto &&tex : mTriggersMgr.GetActionMap())
            mAction->AppendItem(tex.first);
        mAction->SelectItem(current);
    }
}

// Construit la fenêtre et les éléments
void ActionsWindow::Fill() {
    // Crée le Layout
    mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    // Labels
    mInfosSectionLabel = sfg::Label::Create(L"--");

    // Chargement de texture
    mNameHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mNameLabel = sfg::Label::Create(L"Nom : ");
    mName = sfg::Entry::Create();
    mNameHBox->PackEnd(mNameLabel, false);
    mNameHBox->PackEnd(mName);
    mPathHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mPathLabel = sfg::Label::Create(L"Chemin : ");
    mPath = sfg::Entry::Create();
    mPathHBox->PackEnd(mPathLabel, false);
    mPathHBox->PackEnd(mPath);
    mFunctionHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mFunctionLabel = sfg::Label::Create(L"Fonction : ");
    mFunction = sfg::Entry::Create();
    mFunctionHBox->PackEnd(mFunctionLabel, false);
    mFunctionHBox->PackEnd(mFunction);
    mCreateBtn = sfg::Button::Create(L"Charger");
    mCreateLabel = sfg::Label::Create(L"--");

    // Action
    mActionHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mActionLabel = sfg::Label::Create(L"Action : ");
    mAction = sfg::ComboBox::Create();
    mActionHBox->PackEnd(mActionLabel, false);
    mActionHBox->PackEnd(mAction);

    // Boutons
    mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mDeleteBtn = sfg::Button::Create(L"Supprimer");
    mRefreshBtn = sfg::Button::Create(L"Actualiser");
    mCloseBtn = sfg::Button::Create(L"Fermer");
    mButtonsHBox->PackEnd(mDeleteBtn);
    mButtonsHBox->PackEnd(mRefreshBtn);
    mButtonsHBox->PackEnd(mCloseBtn);

    // Signaux
    mCreateBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ActionsWindow::OnLoadAction, this));
    mDeleteBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ActionsWindow::OnDeleteAction, this));
    mRefreshBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ActionsWindow::OnRefresh, this));
    mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ActionsWindow::OnClose, this));

    // Ajoute les éléments à la fenêtre
    mVBox->PackEnd(mNameHBox);
    mVBox->PackEnd(mPathHBox);
    mVBox->PackEnd(mCreateLabel);
    mVBox->PackEnd(mCreateBtn);
    mVBox->PackEnd(mInfosSectionLabel);
    mVBox->PackEnd(mActionHBox);
    mVBox->PackEnd(mButtonsHBox);

    // Ajoute la mVBox à la fenêtre
    AddToWindow(mVBox);
}

// Callbacks
void ActionsWindow::OnLoadAction() {
    if (!mApply) return;

    std::string name = mName->GetText();
    std::string path = mPath->GetText();
    std::string function = mFunction->GetText();

    // Si les entrées sont invalides, on arrête
    if (name.empty() || path.empty()) {
        mCreateLabel->SetText(L"Entrées invalides.");
        return;
    }

    // Charge l'action
    if (function.empty()) {
        mTriggersMgr.AddAction(new ScriptAction(name, path));
    } else {
        mTriggersMgr.AddAction(new ScriptAction(name, path, function));
    }
    mCreateLabel->SetText(L"Action créée.");

    OnRefresh();
}

void ActionsWindow::OnDeleteAction() {
    if (!mApply || mAction->GetSelectedItem() == sfg::ComboBox::NONE) return;

    mTriggersMgr.DeleteAction(mAction->GetSelectedText());

    OnRefresh();
}
