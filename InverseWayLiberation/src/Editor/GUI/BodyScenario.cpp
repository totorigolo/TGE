#include "BodyScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Tools/utils.h"

// Ctor
BodyScenario::BodyScenario(EditBox &editBox)
	: Scenario(editBox, "Body"),
	mEntityMgr(EntityManager::GetInstance()), mResourceMgr(ResourceManager::GetInstance()),
	mSelection(nullptr)
{
	// Initialise le pas
	mPosStepSaveValue = 1.f;

	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de la sélection
void BodyScenario::Select(BaseBody *selection)
{
	myAssert(selection, "BasicBody passé invalide.");

	mSelection = selection;

	OnRefresh();
}
void BodyScenario::Unselect()
{
	mSelection = nullptr;
}

// Actualisation
void BodyScenario::Update()
{
	if (!mSelection) return;
	mApply = false;

	// Met à jour les valeurs
	mPosX->SetText(Parser::floatToString(mSelection->GetPosition().x, 4));
	mPosY->SetText(Parser::floatToString(mSelection->GetPosition().y, 4));
	float rot = mSelection->GetRotationD(); rot = float(int(rot) % 360) + rot - float(int(rot));
	mRot->SetText(Parser::floatToString(rot, 4));

	// Gère le type
	if (mSelection->Getb2BodyType() == b2BodyType::b2_dynamicBody)
		mType[0]->SetActive(true);
	else if (mSelection->Getb2BodyType() == b2BodyType::b2_staticBody)
		mType[1]->SetActive(true);
	else
		myThrowError("Type de BaseBody non géré.");

	// Gère le type de collision
	if (mSelection->GetCollisionType() == BaseBody::CollisionType::Default)
		mCollisionType->SelectItem(0);
	else if (mSelection->GetCollisionType() == BaseBody::CollisionType::Bullet)
		mCollisionType->SelectItem(1);
	else if (mSelection->GetCollisionType() == BaseBody::CollisionType::OneSidedPlatform)
		mCollisionType->SelectItem(2);
	else
		myThrowError("CollisionType de BaseBody non géré.");

	// Mets à jour la liste de texture, et la texture actuelle
	for (int i = mTexture->GetItemCount(); i > 0; --i)
		mTexture->RemoveItem(i - 1);
	int index = 0;
	for (auto &&tex : mResourceMgr.GetTextureMap())
	{
		mTexture->AppendItem(tex.first);

		if (tex.first == mSelection->GetTexture()->GetName())
			mTexture->SelectItem(index);
		++index;
	}

	// Gère les paramètres de collision
	mDensity->SetValue(mSelection->GetDensity());
	mFriction->SetValue(mSelection->GetFriction());
	mRestitution->SetValue(mSelection->GetRestitution());

	// Gère le Layer
	mLayer->SetValue(static_cast<float>(mSelection->GetLayer()));

	// Mets à jour pour les ombres
	if (mSelection->IsActiveShadows())
		mShadows[0]->SetActive(true);
	else
		mShadows[1]->SetActive(true);

	mApply = true;
}

// Construit la fenêtre et les éléments
void BodyScenario::Fill()
{
	// Position et rotation
	mPosTable = sfg::Table::Create();
	mPosLabel = sfg::Label::Create(L"PosRot :");
	mPosX = sfg::Entry::Create();
	mPosX->SetRequisition(sf::Vector2f(60.f, 0.f));
	mPosY = sfg::Entry::Create();
	mPosY->SetRequisition(sf::Vector2f(60.f, 0.f));
	mRot = sfg::Entry::Create();
	mRot->SetRequisition(sf::Vector2f(60.f, 0.f));
	mPosButton = sfg::Button::Create(L"X");
	mPosButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosition, this));
	mPosStep = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
	mPosStep->SetValue(mPosStepSaveValue);
	mPosStep->SetDigits(1);
	mPosStep->SetRequisition(sf::Vector2f(40.f, 0.f));
	mPosXp = sfg::Button::Create(L"+");
	mPosXp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosXp, this));
	mPosXm = sfg::Button::Create(L"-");
	mPosXm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosXm, this));
	mPosYp = sfg::Button::Create(L"+");
	mPosYp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosYp, this));
	mPosYm = sfg::Button::Create(L"-");
	mPosYm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosYm, this));
	mPosRp = sfg::Button::Create(L"+");
	mPosRp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosRp, this));
	mPosRm = sfg::Button::Create(L"-");
	mPosRm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosRm, this));
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

	// Type
	mTypeTable = sfg::Table::Create();
	mTypeTable->SetRequisition(sf::Vector2f(0, 30.f));
	mTypeLabel = sfg::Label::Create(L"Type :");
	mType.resize(2);//(3);
	mType[0] = sfg::RadioButton::Create("Dynamique");
	mType[1] = sfg::RadioButton::Create("Statique", mType[0]->GetGroup());
	//mType[2] = sfg::RadioButton::Create("Kinématique", mType[1]->GetGroup());
	mType[0]->GetSignal(sfg::ComboBox::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangeType, this));
	mType[1]->GetSignal(sfg::ComboBox::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangeType, this));
	mTypeTable->Attach(mTypeLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mTypeTable->Attach(mType[0], sf::Rect<sf::Uint32>(2, 1, 5, 1));
	mTypeTable->Attach(mType[1], sf::Rect<sf::Uint32>(8, 1, 5, 1));

	// CollisionType
	mCollisionTypeTable = sfg::Table::Create();
	mCollisionTypeLabel = sfg::Label::Create(L"ColType : ");
	mCollisionType = sfg::ComboBox::Create();
	mCollisionType->AppendItem("Défaut");
	mCollisionType->AppendItem("Bullet");
	mCollisionType->AppendItem("OneSidedPlatform");
	mCollisionType->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&BodyScenario::OnChangeCollisionType, this));
	mCollisionTypeTable->Attach(mCollisionTypeLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mCollisionTypeTable->Attach(mCollisionType, sf::Rect<sf::Uint32>(2, 1, 5, 1));

	// Paramètres de collision
	mPhysicsParamsTable = sfg::Table::Create();
	mDensityLabel = sfg::Label::Create(L"Densité :");
	mDensity = sfg::SpinButton::Create(0.f, 100.f, 0.02f);
	mDensity->SetDigits(2);
	mDensity->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BodyScenario::OnChangeDensity, this));
	mFrictionLabel = sfg::Label::Create(L"Friction :");
	mFriction = sfg::SpinButton::Create(0.f, 100.f, 0.02f);
	mFriction->SetDigits(2);
	mFriction->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BodyScenario::OnChangeFriction, this));
	mRestitutionLabel = sfg::Label::Create(L"Restitution :");
	mRestitution = sfg::SpinButton::Create(0.f, 100.f, 0.02f);
	mRestitution->SetDigits(2);
	mRestitution->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BodyScenario::OnChangeRestitution, this));
	mPhysicsParamsTable->Attach(mDensityLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mPhysicsParamsTable->Attach(mDensity, sf::Rect<sf::Uint32>(2, 1, 1, 1));
	mPhysicsParamsTable->Attach(mFrictionLabel, sf::Rect<sf::Uint32>(1, 2, 1, 1));
	mPhysicsParamsTable->Attach(mFriction, sf::Rect<sf::Uint32>(2, 2, 1, 1));
	mPhysicsParamsTable->Attach(mRestitutionLabel, sf::Rect<sf::Uint32>(1, 3, 1, 1));
	mPhysicsParamsTable->Attach(mRestitution, sf::Rect<sf::Uint32>(2, 3, 1, 1));

	// Texture
	mTextureHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mTextureLabel = sfg::Label::Create(L"Texture :");
	mTexture = sfg::ComboBox::Create();
	mTexture->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&BodyScenario::OnChangeTexture, this));
	mTextureHBox->PackEnd(mTextureLabel);
	mTextureHBox->PackEnd(mTexture);

	// Layer
	mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mLayerLabel = sfg::Label::Create(L"Layer : ");
	mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
	mLayer->SetValue(1);
	mLayer->SetDigits(0);
	mLayer->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BodyScenario::OnChangeLayer, this));
	mLayerHBox->PackEnd(mLayerLabel, false);
	mLayerHBox->PackEnd(mLayer);

	// Ombres
	mShadowsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mShadowsLabel = sfg::Label::Create(L"Ombres :");
	mShadows.resize(2);
	mShadows[0] = sfg::RadioButton::Create("Oui");
	mShadows[0]->GetSignal(sfg::RadioButton::OnToggle).Connect(std::bind(&BodyScenario::OnChangeShadows, this));
	mShadows[1] = sfg::RadioButton::Create("Non", mShadows[0]->GetGroup());
	mShadows[1]->GetSignal(sfg::RadioButton::OnToggle).Connect(std::bind(&BodyScenario::OnChangeShadows, this));
	mShadowsHBox->PackEnd(mShadowsLabel);
	mShadowsHBox->PackEnd(mShadows[0]);
	mShadowsHBox->PackEnd(mShadows[1]);

	// Bouton Détruire & ClearForces
	mButtonsHBox1 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mDestroy = sfg::Button::Create(L"Détruire");
	mDestroy->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnDestroy, this));
	mClearForces = sfg::Button::Create(L"ClearForces");
	mClearForces->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnClearForces, this));
	mButtonsHBox1->PackEnd(mDestroy);
	mButtonsHBox1->PackEnd(mClearForces);

	// Bouton d'actualisation & de Collision Masking
	mButtonsHBox2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mRefresh = sfg::Button::Create(L"Actualiser");
	mRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnRefresh, this));
	mColFilteringButton = sfg::Button::Create(L"Col Filtering");
	mColFilteringButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowColFilteringWindow, &mEditBox));
	mButtonsHBox2->PackEnd(mColFilteringButton);
	mButtonsHBox2->PackEnd(mRefresh);

	// Ajoute les éléments à la fenêtre
	AddToVBox(mPosTable);
	AddToVBox(mTypeTable);
	AddToVBox(mCollisionTypeTable);
	AddToVBox(mPhysicsParamsTable);
	AddToVBox(mTextureHBox);
	AddToVBox(mLayerHBox);
	AddToVBox(mShadowsHBox);
	AddToVBox(mButtonsHBox1);
	AddToVBox(mButtonsHBox2);
}

// Callbacks
void BodyScenario::OnChangePosition()
{
	if (!mApply || !mSelection) return;

	// Change sa position et sa rotation
	mSelection->SetTransform(b2Vec2(Parser::stringToFloat(mPosX->GetText().toAnsiString()), Parser::stringToFloat(mPosY->GetText().toAnsiString())),
		Parser::stringToFloat(mRot->GetText().toAnsiString()) * RPD);
}
void BodyScenario::OnChangeType()
{
	if (!mApply || !mSelection) return;

	if (mType[0]->IsActive()) mSelection->Setb2BodyType(b2BodyType::b2_dynamicBody);
	else if (mType[1]->IsActive()) mSelection->Setb2BodyType(b2BodyType::b2_staticBody);
	//else if (mType[2]->IsActive()) mSelection->Setb2BodyType(b2BodyType::b2_kinematicBody);
}
void BodyScenario::OnChangeCollisionType()
{
	if (!mApply || !mSelection) return;

	int ntype = mCollisionType->GetSelectedItem();
	if (ntype == 0) mSelection->SetCollisionType(BaseBody::CollisionType::Default);
	else if (ntype == 1) mSelection->SetCollisionType(BaseBody::CollisionType::Bullet);
	else if (ntype == 2) mSelection->SetCollisionType(BaseBody::CollisionType::OneSidedPlatform);
}
void BodyScenario::OnChangeTexture()
{
	if (!mApply || !mSelection) return;

	mSelection->SetTexture(mResourceMgr.GetTexture(mTexture->GetItem(mTexture->GetSelectedItem()).toAnsiString()));
}
void BodyScenario::OnChangeLayer()
{
	if (!mApply || !mSelection) return;

	mSelection->SetLayer(static_cast<int>(mLayer->GetValue()));
}
void BodyScenario::OnChangeShadows()
{
	if (!mApply || !mSelection) return;

	// Change les ombres
	mSelection->SetShadowsActive(mShadows[0]->IsActive());
	Update();
}
void BodyScenario::OnChangePosXp()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(-mPosStep->GetValue(), 0.f), mSelection->GetRotationR());

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosXm()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(mPosStep->GetValue(), 0.f), mSelection->GetRotationR());

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosYp()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(0.f, -mPosStep->GetValue()), mSelection->GetRotationR());

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosYm()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetTransform(mSelection->GetPosition() - b2Vec2(0.f, mPosStep->GetValue()), mSelection->GetRotationR());

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosRp()
{
	if (!mApply || !mSelection) return;

	// Change sa rotation
	mSelection->SetTransform(mSelection->GetPosition(), mSelection->GetRotationR() - 2 * RPD);

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosRm()
{
	if (!mApply || !mSelection) return;

	// Change sa position
	mSelection->SetTransform(mSelection->GetPosition(), mSelection->GetRotationR() + 2 * RPD);

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangeDensity()
{
	if (!mApply || !mSelection) return;

	// Gère les paramètres de collision
	mSelection->SetDensity(mDensity->GetValue());
}
void BodyScenario::OnChangeFriction()
{
	if (!mApply || !mSelection) return;

	// Gère les paramètres de collision
	mSelection->SetFriction(mFriction->GetValue());
}
void BodyScenario::OnChangeRestitution()
{
	if (!mApply || !mSelection) return;

	// Gère les paramètres de collision
	mSelection->SetRestitution(mRestitution->GetValue());
}
void BodyScenario::OnClearForces()
{
	if (!mApply || !mSelection) return;

	// Obtient le BasicBody
	myAssert(mSelection->GetBody(), "Aucun b2Body associé au BaseBody.");
	mSelection->GetBody()->SetLinearVelocity(b2Vec2_zero);
	mSelection->GetBody()->SetAngularVelocity(0.f);
}
void BodyScenario::OnDestroy()
{
	if (!mApply || !mSelection) return;

	// Détruit le BaseBody
	mEntityMgr.DestroyEntity(mSelection);
	Unselect();
	mEditBox.ScheduleUnselection();
}
