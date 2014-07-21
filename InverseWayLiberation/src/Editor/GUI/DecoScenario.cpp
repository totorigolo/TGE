#include "stdafx.h"
#include "DecoScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"

// Ctor
DecoScenario::DecoScenario(EditBox &editBox)
	: Scenario(editBox, "Deco"),
	mEntityMgr(EntityManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()), mResourceMgr(ResourceManager::GetInstance()),
	mSelection(nullptr)
{
	// Initialise le pas
	mPosStepSaveValue = 1.f;

	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de la sélection
void DecoScenario::Select(Deco *selection)
{
	myAssert(selection, "BasicBody passé invalide.");

	mSelection = selection;

	OnRefresh();
}
void DecoScenario::Unselect()
{
	mSelection = nullptr;
}

// Actualisation
void DecoScenario::Update()
{
	if (!mSelection) return;
	mApply = false;

	// Met à jour les valeurs
	mPosX->SetText(Parser::floatToString(mSelection->GetPosition().x, 4));
	mPosY->SetText(Parser::floatToString(mSelection->GetPosition().y, 4));
	float rot = mSelection->GetRotationD(); rot = float(int(rot) % 360) + rot - float(int(rot));
	mRot->SetText(Parser::floatToString(rot, 4));

	// Gère le Layer
	mLayer->SetValue(static_cast<float>(mSelection->GetLayer()));

	// Mets à jour la liste de texture, et la texture actuelle
	for (int i = mTexture->GetItemCount(); i > 0; --i)
		mTexture->RemoveItem(i - 1);
	int index = 0;
	for each (const auto &tex in mResourceMgr.GetTextureMap())
	{
		mTexture->AppendItem(tex.first);

		if (tex.first == mSelection->GetTexture()->GetName())
			mTexture->SelectItem(index);
		++index;
	}

	// Mets à jour pour les ombres
	if (mSelection->IsActiveShadows())
		mShadows[0]->SetActive(true);
	else
		mShadows[1]->SetActive(true);

	mApply = true;
}

// Construit la fenêtre et les éléments
void DecoScenario::Fill()
{
	// Position et rotation
	mPosTable = sfg::Table::Create();
	mPosLabel = sfg::Label::Create("PosRot :");
	mPosX = sfg::Entry::Create();
	mPosX->SetRequisition(sf::Vector2f(60.f, 0.f));
	mPosY = sfg::Entry::Create();
	mPosY->SetRequisition(sf::Vector2f(60.f, 0.f));
	mRot = sfg::Entry::Create();
	mRot->SetRequisition(sf::Vector2f(60.f, 0.f));
	mPosButton = sfg::Button::Create("X");
	mPosButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnChangePosition, this));
	mPosStep = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
	mPosStep->SetValue(mPosStepSaveValue);
	mPosStep->SetDigits(1);
	mPosStep->SetRequisition(sf::Vector2f(40.f, 0.f));
	mPosXp = sfg::Button::Create("+");
	mPosXp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnChangePosXp, this));
	mPosXm = sfg::Button::Create("-");
	mPosXm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnChangePosXm, this));
	mPosYp = sfg::Button::Create("+");
	mPosYp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnChangePosYp, this));
	mPosYm = sfg::Button::Create("-");
	mPosYm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnChangePosYm, this));
	mPosRp = sfg::Button::Create("+");
	mPosRp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnChangePosRp, this));
	mPosRm = sfg::Button::Create("-");
	mPosRm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnChangePosRm, this));
	mPosTable->Attach(mPosLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mPosTable->Attach(mPosX, sf::Rect<sf::Uint32>(2, 1, 4, 1));
	mPosTable->Attach(mPosY, sf::Rect<sf::Uint32>(6, 1, 4, 1));
	mPosTable->Attach(mRot, sf::Rect<sf::Uint32>(10, 1, 4, 1));
	mPosTable->Attach(mPosButton, sf::Rect<sf::Uint32>(14, 1, 1, 1));
	mPosTable->Attach(mPosStep, sf::Rect<sf::Uint32>(1, 2, 1, 1));
	mPosTable->Attach(mPosXm, sf::Rect<sf::Uint32>(2, 2, 2, 1));
	mPosTable->Attach(mPosXp, sf::Rect<sf::Uint32>(4, 2, 2, 1));
	mPosTable->Attach(mPosYp, sf::Rect<sf::Uint32>(6, 2, 2, 1));
	mPosTable->Attach(mPosYm, sf::Rect<sf::Uint32>(8, 2, 2, 1));
	mPosTable->Attach(mPosRp, sf::Rect<sf::Uint32>(10, 2, 2, 1));
	mPosTable->Attach(mPosRm, sf::Rect<sf::Uint32>(12, 2, 2, 1));

	// Layer
	mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mLayerLabel = sfg::Label::Create("Layer : ");
	mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
	mLayer->SetValue(5);
	mLayer->SetDigits(0);
	mLayer->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DecoScenario::OnChangeLayer, this));
	mLayerHBox->PackEnd(mLayerLabel, false);
	mLayerHBox->PackEnd(mLayer);

	// Texture
	mTextureHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mTextureLabel = sfg::Label::Create("Texture :");
	mTexture = sfg::ComboBox::Create();
	mTexture->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DecoScenario::OnChangeTexture, this));
	mTextureHBox->PackEnd(mTextureLabel, false);
	mTextureHBox->PackEnd(mTexture);

	// Ombres
	mShadowsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mShadowsLabel = sfg::Label::Create("Ombres :");
	mShadows.resize(2);
	mShadows[0] = sfg::RadioButton::Create("Oui");
	mShadows[0]->GetSignal(sfg::RadioButton::OnToggle).Connect(std::bind(&DecoScenario::OnChangeShadows, this));
	mShadows[1] = sfg::RadioButton::Create("Non", mShadows[0]->GetGroup());
	mShadows[1]->GetSignal(sfg::RadioButton::OnToggle).Connect(std::bind(&DecoScenario::OnChangeShadows, this));
	mShadowsHBox->PackEnd(mShadowsLabel);
	mShadowsHBox->PackEnd(mShadows[0]);
	mShadowsHBox->PackEnd(mShadows[1]);

	// Boutons
	mRefresh = sfg::Button::Create("Actualiser");
	mRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnRefresh, this));
	mDestroy = sfg::Button::Create("Détruire");
	mDestroy->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DecoScenario::OnDestroy, this));

	// Ajoute les éléments à la fenêtre
	AddToVBox(mPosTable);
	AddToVBox(mLayerHBox);
	AddToVBox(mTextureHBox);
	AddToVBox(mShadowsHBox);
	AddToVBox(mRefresh);
	AddToVBox(mDestroy);
}

// Callbacks
void DecoScenario::OnChangePosition()
{
	if (!mApply || !mSelection) return;

	// Change sa position et sa rotation
	mSelection->SetPosition(b2Vec2(Parser::stringToFloat(mPosX->GetText()), Parser::stringToFloat(mPosY->GetText())));
	mSelection->SetRotationD(Parser::stringToFloat(mRot->GetText()));
	Update();
}
void DecoScenario::OnChangeTexture()
{
	if (!mApply || !mSelection) return;

	mSelection->SetTexture(mResourceMgr.GetTexture(mTexture->GetItem(mTexture->GetSelectedItem())));
	Update();
}
void DecoScenario::OnChangeLayer()
{
	if (!mApply || !mSelection) return;

	mSelection->SetLayer(static_cast<int>(mLayer->GetValue()));
	Update();
}
void DecoScenario::OnChangeShadows()
{
	if (!mApply || !mSelection) return;

	// Change les ombres
	mSelection->SetShadowsActive(mShadows[0]->IsActive());
	Update();
}
void DecoScenario::OnChangePosXp()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetPosition(mSelection->GetPositionPx() - sf::Vector2f(-mPosStep->GetValue() * mPhysicMgr.GetPPM(), 0.f));
	Update();
}
void DecoScenario::OnChangePosXm()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetPosition(mSelection->GetPositionPx() - sf::Vector2f(mPosStep->GetValue() * mPhysicMgr.GetPPM(), 0.f));
	Update();
}
void DecoScenario::OnChangePosYp()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetPosition(mSelection->GetPositionPx() - sf::Vector2f(0.f, mPosStep->GetValue() * mPhysicMgr.GetPPM()));
	Update();
}
void DecoScenario::OnChangePosYm()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetPosition(mSelection->GetPositionPx() - sf::Vector2f(0.f, -mPosStep->GetValue() * mPhysicMgr.GetPPM()));
	Update();
}
void DecoScenario::OnChangePosRp()
{
	if (!mApply || !mSelection) return;

	// Change sa rotation
	mSelection->SetRotationD(mSelection->GetRotationD() + 2);
	Update();
}
void DecoScenario::OnChangePosRm()
{
	if (!mApply || !mSelection) return;

	// Change sa rotation
	mSelection->SetRotationD(mSelection->GetRotationD() - 2);
	Update();
}
void DecoScenario::OnDestroy()
{
	if (!mApply || !mSelection) return;

	// Détruit le BasicBody
	mEntityMgr.DestroyEntity(mSelection);
	Unselect();
	mEditBox.ScheduleUnselection();
}
