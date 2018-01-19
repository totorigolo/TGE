#include "DecoCreationWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/EntityFactory.h"
#include "../../Tools/utils.h"

// Ctor
DecoCreationWindow::DecoCreationWindow()
        : Window("DecoCreation"),
          mIsInAddMode(false),
          mEntityMgr(EntityManager::GetInstance()),
          mPhysicMgr(PhysicManager::GetInstance()),
          mResourceMgr(ResourceManager::GetInstance())
{
    // Rempli la fenêtre
    Fill();
    mApply = true;
}

// Gestion de l'ajout
void DecoCreationWindow::Add(b2Vec2 pos) {
    if (!mApply || mTexture->GetSelectedItem() == sfg::ComboBox::NONE) return;

    // Sort du mode création si la fenêtre est cachée
    if (!this->IsVisible() || !mIsInAddMode) {
        OnToggleMode();
        return;
    }

    // Il nous faut une texture
    if (mTexture->GetSelectedItem() == sfg::ComboBox::NONE) {
        std::cout << "Aucune texture choisie !" << std::endl;
        return;
    }

    // Transforme pos en b2Vec3
    b2Vec3 posRot = getVec3(pos);

    // Crée la déco
    EntityFactory::CreateDeco(posRot, mTexture->GetItem(mTexture->GetSelectedItem()).toAnsiString(),
                              static_cast<int>(mLayer->GetValue()));

    OnRefresh();
}

// Actualisation
void DecoCreationWindow::Update() {
    // Mets à jour la liste de texture
    if (mTexture.get()) {
        auto current = mTexture->GetSelectedItem();
        for (int i = mTexture->GetItemCount(); i > 0; --i)
            mTexture->RemoveItem(i - 1);
        for (auto &&tex : mResourceMgr.GetTextureMap())
            mTexture->AppendItem(tex.first);
        mTexture->SelectItem(current);
    }
}

// Accesseurs
bool DecoCreationWindow::IsInAddMode() {
    return mIsInAddMode;
}

// Construit la fenêtre et les éléments
void DecoCreationWindow::Fill() {
    // Crée le Layout
    mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    // Eléments
    mMode = sfg::Label::Create(L"Mode : Attente");
    mHelpLabel = sfg::Label::Create(L"Créer déco : Ctrl + clic gauche");

    // Texture
    mTextureHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mTextureLabel = sfg::Label::Create(L"Texture : ");
    mTexture = sfg::ComboBox::Create();
    mTextureHBox->PackEnd(mTextureLabel, false);
    mTextureHBox->PackEnd(mTexture);

    // Layer
    mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mLayerLabel = sfg::Label::Create(L"Layer : ");
    mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
    mLayer->SetValue(5);
    mLayer->SetDigits(0);
    mLayerHBox->PackEnd(mLayerLabel, false);
    mLayerHBox->PackEnd(mLayer);

    // Boutons
    mToggleModeBtn = sfg::Button::Create(L"Entrer mode Création");
    mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mRefreshBtn = sfg::Button::Create(L"Actualiser");
    mCloseBtn = sfg::Button::Create(L"Fermer");
    mButtonsHBox->PackEnd(mRefreshBtn);
    mButtonsHBox->PackEnd(mCloseBtn);

    // Signaux
    mToggleModeBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoCreationWindow::OnToggleMode, this));
    mRefreshBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoCreationWindow::OnRefresh, this));
    mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoCreationWindow::OnClose, this));

    // Ajoute les éléments à la fenêtre
    mVBox->PackEnd(mMode);
    mVBox->PackEnd(mHelpLabel);
    mVBox->PackEnd(mTextureHBox);
    mVBox->PackEnd(mLayerHBox);
    mVBox->PackEnd(mToggleModeBtn);
    mVBox->PackEnd(mButtonsHBox);

    // Ajoute la mVBox à la fenêtre
    AddToWindow(mVBox);
}

// Callbacks
void DecoCreationWindow::OnToggleMode() {
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
