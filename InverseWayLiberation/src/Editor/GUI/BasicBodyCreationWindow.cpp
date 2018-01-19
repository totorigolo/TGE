#include "BasicBodyCreationWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/PolyBody.h"
#include "../../Entities/EntityFactory.h"
#include "../../Tools/utils.h"

// Ctor
BasicBodyCreationWindow::BasicBodyCreationWindow()
	: Window("BasicBodyCreation"),
	mEntityMgr(EntityManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()), mResourceMgr(ResourceManager::GetInstance()),
	mIsInAddMode(false)
{
	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de l'ajout
void BasicBodyCreationWindow::Add(b2Vec2 pos)
{
	if (!mApply || mTexture->GetSelectedItem() == sfg::ComboBox::NONE) return;

	// Sort du mode création si la fenêtre est cachée
	if (!this->IsVisible() || !mIsInAddMode)
	{
		OnToggleMode();
		return;
	}

	// Il nous faut une texture
	if (mTexture->GetSelectedItem() == sfg::ComboBox::NONE)
	{
		std::cout << "Aucune texture choisie !" << std::endl;
		return;
	}

	// Transforme pos en b2Vec3
	b2Vec3 posRot = getVec3(pos);

	// Récupère le type
	b2BodyType type = b2BodyType::b2_dynamicBody;
	if (mType[0]->IsActive()) // Dynamique
		type = b2BodyType::b2_dynamicBody;
	else if (mType[1]->IsActive()) // Static
		type = b2BodyType::b2_staticBody;
	else if (mType[2]->IsActive()) // Kinématique
		type = b2BodyType::b2_kinematicBody;
	else
	{
		std::cout << "Aucun type choisi !" << std::endl;
		return;
	}

	// Crée le Body suivant la forme
	if (mShape[0]->IsActive()) // Box
		EntityFactory::CreateBox(posRot, type, mTexture->GetItem(mTexture->GetSelectedItem()).toAnsiString(), static_cast<int>(mLayer->GetValue()));
	else if (mShape[1]->IsActive()) // Circle
		EntityFactory::CreateCircle(posRot, type, mTexture->GetItem(mTexture->GetSelectedItem()).toAnsiString(), static_cast<int>(mLayer->GetValue()));
	else
	{
		std::cout << "Aucune forme choisie !" << std::endl;
		return;
	}

	OnRefresh();
}

// Actualisation
void BasicBodyCreationWindow::Update()
{
	// Mets à jour la liste de texture
	if (mTexture.get())
	{
		auto current = mTexture->GetSelectedItem();
		for (int i = mTexture->GetItemCount(); i > 0; --i)
			mTexture->RemoveItem(i - 1);
		for (auto &&tex : mResourceMgr.GetTextureMap())
			mTexture->AppendItem(tex.first);
		mTexture->SelectItem(current);
	}
}

// Accesseurs
bool BasicBodyCreationWindow::IsInAddMode()
{
	return mIsInAddMode;
}

// Construit la fenêtre et les éléments
void BasicBodyCreationWindow::Fill()
{
	// Crée le Layout
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// Eléments
	mMode = sfg::Label::Create(L"Mode : Attente");
	mHelpLabel = sfg::Label::Create(L"Créer body : Ctrl + clic gauche");

	// Forme
	mShapeBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mShapeLabel = sfg::Label::Create(L"Forme : ");
	mShape.resize(2);
	mShape[0] = sfg::RadioButton::Create("Rectangle");
	mShape[1] = sfg::RadioButton::Create("Cercle", mShape[0]->GetGroup());
	mShapeBox->PackEnd(mShapeLabel, false);
	mShapeBox->PackEnd(mShape[0]);
	mShapeBox->PackEnd(mShape[1]);

	// Type
	mTypeBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mTypeLabel = sfg::Label::Create(L"Type : ");
	mType.resize(3);
	mType[0] = sfg::RadioButton::Create("Dynamique");
	mType[1] = sfg::RadioButton::Create("Statique", mType[0]->GetGroup());
	mType[2] = sfg::RadioButton::Create("Kinématique", mType[1]->GetGroup());
	mTypeBox->PackEnd(mTypeLabel, false);
	mTypeBox->PackEnd(mType[0]);
	mTypeBox->PackEnd(mType[1]);

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
	mLayer->SetValue(1);
	mLayer->SetDigits(0);
	mLayerHBox->PackEnd(mLayerLabel, false);
	mLayerHBox->PackEnd(mLayer);

	// Boutons
	mToggleModeBtn = sfg::Button::Create(L"Entrer mode Création");
	mCloseBtn = sfg::Button::Create(L"Fermer");

	// Signaux
	mToggleModeBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BasicBodyCreationWindow::OnToggleMode, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BasicBodyCreationWindow::OnClose, this));

	// Ajoute les éléments à la fenêtre
	mVBox->PackEnd(mMode);
	mVBox->PackEnd(mHelpLabel);
	mVBox->PackEnd(mShapeBox);
	mVBox->PackEnd(mTypeBox);
	mVBox->PackEnd(mTextureHBox);
	mVBox->PackEnd(mLayerHBox);
	mVBox->PackEnd(mToggleModeBtn);
	mVBox->PackEnd(mCloseBtn);

	// Ajoute la mVBox à la fenêtre
	AddToWindow(mVBox);
}

// Callbacks
void BasicBodyCreationWindow::OnToggleMode()
{
	if (!mApply) return;

	mIsInAddMode = !mIsInAddMode;

	if (mIsInAddMode)
	{
		mMode->SetText(L"Mode : Création");
		mToggleModeBtn->SetLabel(L"Sortir mode Création");
	}
	else
	{
		mMode->SetText(L"Mode : Attente");
		mToggleModeBtn->SetLabel(L"Entrer mode Création");
	}
}
