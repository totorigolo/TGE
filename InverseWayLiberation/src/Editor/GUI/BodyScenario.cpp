#include "BodyScenario.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Tools/Error.h"
#include "../../Tools/utils.h"

// Ctor
BodyScenario::BodyScenario(EditBox &editBox)
	: Scenario(editBox, "BasicBody"),
	mEntityMgr(EntityManager::GetInstance()),
	mSelectionPoly(nullptr), mSelectionBasic(nullptr)
{
	// Initialise le pas
	mPosStepSaveValue = 1.f;

	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de la sélection
void BodyScenario::Select(PolyBody *selection)
{
	myAssert(selection, "PolyBody passé invalide.");

	mSelectionBasic = nullptr;
	mSelectionPoly = selection;

	OnRefresh();
}
void BodyScenario::Select(BasicBody *selection)
{
	myAssert(selection, "BasicBody passé invalide.");

	mSelectionPoly = nullptr;
	mSelectionBasic = selection;

	OnRefresh();
}
void BodyScenario::Unselect()
{
	mSelectionPoly = nullptr;
	mSelectionBasic = nullptr;
}

// Actualisation
void BodyScenario::Update()
{
	if (mSelectionPoly)
	{
		// Met à jour les valeurs
		mPosX->SetText(Parser::floatToString(mSelectionPoly->GetPosition().x, 4));
		mPosY->SetText(Parser::floatToString(mSelectionPoly->GetPosition().y, 4));
		float rot = mSelectionPoly->GetRotationD(); rot = float(int(rot) % 360) + rot - float(int(rot));
		mRot->SetText(Parser::floatToString(rot, 4));

		// Gère le type
		if (mSelectionPoly->Getb2BodyType() == b2BodyType::b2_dynamicBody)
			mType[0]->SetActive(true);
		else if (mSelectionPoly->Getb2BodyType() == b2BodyType::b2_staticBody)
			mType[1]->SetActive(true);
		else
			myThrowError("Type de PolyBody non géré.");

		// Gère le type de collision
		if (mSelectionPoly->GetCollisionType() == PolyBody::CollisionType::Default)
			mCollisionType->SelectItem(0);
		else if (mSelectionPoly->GetCollisionType() == PolyBody::CollisionType::Bullet)
			mCollisionType->SelectItem(1);
		else if (mSelectionPoly->GetCollisionType() == PolyBody::CollisionType::OneSidedPlatform)
			mCollisionType->SelectItem(2);
		else
			myThrowError("CollisionType de PolyBody non géré.");

		// Gère les paramètres de collision
		mDensity->SetValue(mSelectionPoly->GetDensity());
		mFriction->SetValue(mSelectionPoly->GetFriction());
		mRestitution->SetValue(mSelectionPoly->GetRestitution());
	}
	else if (mSelectionBasic)
	{
		// Met à jour les valeurs
		mPosX->SetText(Parser::floatToString(mSelectionBasic->GetPosition().x, 4));
		mPosY->SetText(Parser::floatToString(mSelectionBasic->GetPosition().y, 4));
		float rot = mSelectionBasic->GetRotationD(); rot = float(int(rot) % 360) + rot - float(int(rot));
		mRot->SetText(Parser::floatToString(rot, 4));

		// Gère le type
		if (mSelectionBasic->Getb2BodyType() == b2BodyType::b2_dynamicBody)
			mType[0]->SetActive(true);
		else if (mSelectionBasic->Getb2BodyType() == b2BodyType::b2_staticBody)
			mType[1]->SetActive(true);
		else
			myThrowError("Type de BasicBody non géré.");

		// Gère le type de collision
		if (mSelectionBasic->GetCollisionType() == BasicBody::CollisionType::Default)
			mCollisionType->SelectItem(0);
		else if (mSelectionBasic->GetCollisionType() == BasicBody::CollisionType::Bullet)
			mCollisionType->SelectItem(1);
		else if (mSelectionBasic->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform)
			mCollisionType->SelectItem(2);
		else
			myThrowError("CollisionType de BasicBody non géré.");

		// Gère les paramètres de collision
		mDensity->SetValue(mSelectionBasic->GetDensity());
		mFriction->SetValue(mSelectionBasic->GetFriction());
		mRestitution->SetValue(mSelectionBasic->GetRestitution());
	}
}

// Construit la fenêtre et les éléments
void BodyScenario::Fill()
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
	mPosButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosition, this));
	mPosStep = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
	mPosStep->SetValue(mPosStepSaveValue);
	mPosStep->SetDigits(1);
	mPosStep->SetRequisition(sf::Vector2f(40.f, 0.f));
	mPosXp = sfg::Button::Create("+");
	mPosXp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosXp, this));
	mPosXm = sfg::Button::Create("-");
	mPosXm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosXm, this));
	mPosYp = sfg::Button::Create("+");
	mPosYp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosYp, this));
	mPosYm = sfg::Button::Create("-");
	mPosYm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosYm, this));
	mPosRp = sfg::Button::Create("+");
	mPosRp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnChangePosRp, this));
	mPosRm = sfg::Button::Create("-");
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
	mTypeLabel = sfg::Label::Create("Type :");
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
	mCollisionTypeLabel = sfg::Label::Create("ColType : ");
	mCollisionType = sfg::ComboBox::Create();
	mCollisionType->AppendItem("Défaut");
	mCollisionType->AppendItem("Bullet");
	mCollisionType->AppendItem("OneSidedPlatform");
	mCollisionType->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&BodyScenario::OnChangeCollisionType, this));
	mCollisionTypeTable->Attach(mCollisionTypeLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mCollisionTypeTable->Attach(mCollisionType, sf::Rect<sf::Uint32>(2, 1, 5, 1));

	// Paramètres de collision
	mPhysicsParamsTable = sfg::Table::Create();
	mDensityLabel = sfg::Label::Create("Densité :");
	mDensity = sfg::SpinButton::Create(0.f, 100.f, 0.2f);
	mDensity->SetDigits(3);
	mDensity->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BodyScenario::OnChangeDensity, this));
	mFrictionLabel = sfg::Label::Create("Friction :");
	mFriction = sfg::SpinButton::Create(0.f, 100.f, 0.1f);
	mFriction->SetDigits(3);
	mFriction->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BodyScenario::OnChangeFriction, this));
	mRestitutionLabel = sfg::Label::Create("Restitution :");
	mRestitution = sfg::SpinButton::Create(0.f, 100.f, 0.1f);
	mRestitution->SetDigits(3);
	mRestitution->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&BodyScenario::OnChangeRestitution, this));
	mPhysicsParamsTable->Attach(mDensityLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mPhysicsParamsTable->Attach(mDensity, sf::Rect<sf::Uint32>(2, 1, 1, 1));
	mPhysicsParamsTable->Attach(mFrictionLabel, sf::Rect<sf::Uint32>(1, 2, 1, 1));
	mPhysicsParamsTable->Attach(mFriction, sf::Rect<sf::Uint32>(2, 2, 1, 1));
	mPhysicsParamsTable->Attach(mRestitutionLabel, sf::Rect<sf::Uint32>(1, 3, 1, 1));
	mPhysicsParamsTable->Attach(mRestitution, sf::Rect<sf::Uint32>(2, 3, 1, 1));

	// Bouton Détruire & ClearForces
	mButtonsHBox1 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mDestroy = sfg::Button::Create("Détruire");
	mDestroy->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnDestroy, this));
	mClearForces = sfg::Button::Create("ClearForces");
	mClearForces->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnClearForces, this));
	mButtonsHBox1->PackEnd(mDestroy);
	mButtonsHBox1->PackEnd(mClearForces);

	// Bouton d'actualisation & de Collision Masking
	mButtonsHBox2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mRefresh = sfg::Button::Create("Actualiser");
	mRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&BodyScenario::OnRefresh, this));
	mColFilteringButton = sfg::Button::Create("Col Filtering");
	mColFilteringButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnShowColFilteringWindow, &mEditBox));
	mButtonsHBox2->PackEnd(mColFilteringButton);
	mButtonsHBox2->PackEnd(mRefresh);

	// Ajoute les éléments à la fenêtre
	AddToVBox(mPosTable);
	AddToVBox(mTypeTable);
	AddToVBox(mCollisionTypeTable);
	AddToVBox(mPhysicsParamsTable);
	AddToVBox(mButtonsHBox1);
	AddToVBox(mButtonsHBox2);
}

// Callbacks
void BodyScenario::OnChangePosition()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Change sa position et sa rotation
		mSelectionPoly->SetTransform(b2Vec2(Parser::stringToFloat(mPosX->GetText()), Parser::stringToFloat(mPosY->GetText())),
			Parser::stringToFloat(mRot->GetText()) * RPD);
	}
	else if (mSelectionBasic)
	{
		// Change sa position et sa rotation
		mSelectionBasic->SetTransform(b2Vec2(Parser::stringToFloat(mPosX->GetText()), Parser::stringToFloat(mPosY->GetText())),
			Parser::stringToFloat(mRot->GetText()) * RPD);
	}
}
void BodyScenario::OnChangeType()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		if (mType[0]->IsActive()) mSelectionPoly->Setb2BodyType(b2BodyType::b2_dynamicBody);
		else if (mType[1]->IsActive()) mSelectionPoly->Setb2BodyType(b2BodyType::b2_staticBody);
		//else if (mType[2]->IsActive()) mSelectionPoly->Setb2BodyType(b2BodyType::b2_kinematicBody);
	}
	else if (mSelectionBasic)
	{
		if (mType[0]->IsActive()) mSelectionBasic->Setb2BodyType(b2BodyType::b2_dynamicBody);
		else if (mType[1]->IsActive()) mSelectionBasic->Setb2BodyType(b2BodyType::b2_staticBody);
		//else if (mType[2]->IsActive()) mSelectionBasic->Setb2BodyType(b2BodyType::b2_kinematicBody);
	}
}
void BodyScenario::OnChangeCollisionType()
{
	if (!mApply) return;

	int ntype = mCollisionType->GetSelectedItem();
	if (mSelectionPoly)
	{
		if (ntype == 0) mSelectionPoly->SetCollisionType(PolyBody::CollisionType::Default);
		else if (ntype == 1) mSelectionPoly->SetCollisionType(PolyBody::CollisionType::Bullet);
		else if (ntype == 2) mSelectionPoly->SetCollisionType(PolyBody::CollisionType::OneSidedPlatform);
	}
	else if (mSelectionBasic)
	{
		if (ntype == 0) mSelectionBasic->SetCollisionType(BasicBody::CollisionType::Default);
		else if (ntype == 1) mSelectionBasic->SetCollisionType(BasicBody::CollisionType::Bullet);
		else if (ntype == 2) mSelectionBasic->SetCollisionType(BasicBody::CollisionType::OneSidedPlatform);
	}
}
void BodyScenario::OnChangePosXp()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Change sa position
		mSelectionPoly->SetTransform(mSelectionPoly->GetPosition() - b2Vec2(-mPosStep->GetValue(), 0.f), mSelectionPoly->GetRotationR());
	}
	else if (mSelectionBasic)
	{
		// Change sa position
		mSelectionBasic->SetTransform(mSelectionBasic->GetPosition() - b2Vec2(-mPosStep->GetValue(), 0.f), mSelectionBasic->GetRotationR());
	}

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosXm()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Change sa position
		mSelectionPoly->SetTransform(mSelectionPoly->GetPosition() - b2Vec2(mPosStep->GetValue(), 0.f), mSelectionPoly->GetRotationR());
	}
	else if (mSelectionBasic)
	{
		// Change sa position
		mSelectionBasic->SetTransform(mSelectionBasic->GetPosition() - b2Vec2(mPosStep->GetValue(), 0.f), mSelectionBasic->GetRotationR());
	}

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosYp()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Change sa position
		mSelectionPoly->SetTransform(mSelectionPoly->GetPosition() - b2Vec2(0.f, -mPosStep->GetValue()), mSelectionPoly->GetRotationR());
	}
	else if (mSelectionBasic)
	{
		// Change sa position
		mSelectionBasic->SetTransform(mSelectionBasic->GetPosition() - b2Vec2(0.f, -mPosStep->GetValue()), mSelectionBasic->GetRotationR());
	}

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosYm()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Change sa position
		mSelectionPoly->SetTransform(mSelectionPoly->GetPosition() - b2Vec2(0.f, mPosStep->GetValue()), mSelectionPoly->GetRotationR());
	}
	else if (mSelectionBasic)
	{
		// Change sa position
		mSelectionBasic->SetTransform(mSelectionBasic->GetPosition() - b2Vec2(0.f, mPosStep->GetValue()), mSelectionBasic->GetRotationR());
	}

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosRp()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Change sa rotation
		mSelectionPoly->SetTransform(mSelectionPoly->GetPosition(), mSelectionPoly->GetRotationR() - 2 * RPD);
	}
	else if (mSelectionBasic)
	{
		// Change sa rotation
		mSelectionBasic->SetTransform(mSelectionBasic->GetPosition(), mSelectionBasic->GetRotationR() - 2 * RPD);
	}

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangePosRm()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Change sa position
		mSelectionPoly->SetTransform(mSelectionPoly->GetPosition(), mSelectionPoly->GetRotationR() + 2 * RPD);
	}
	else if (mSelectionBasic)
	{
		// Change sa position
		mSelectionBasic->SetTransform(mSelectionBasic->GetPosition(), mSelectionBasic->GetRotationR() + 2 * RPD);
	}

	// Actualise
	OnRefresh();
}
void BodyScenario::OnChangeDensity()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Gère les paramètres de collision
		mSelectionPoly->SetDensity(mDensity->GetValue());
	}
	else if (mSelectionBasic)
	{
		// Gère les paramètres de collision
		mSelectionBasic->SetDensity(mDensity->GetValue());
	}
}
void BodyScenario::OnChangeFriction()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Gère les paramètres de collision
		mSelectionPoly->SetFriction(mFriction->GetValue());
	}
	else if (mSelectionBasic)
	{
		// Gère les paramètres de collision
		mSelectionBasic->SetFriction(mFriction->GetValue());
	}
}
void BodyScenario::OnChangeRestitution()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Gère les paramètres de collision
		mSelectionPoly->SetRestitution(mRestitution->GetValue());
	}
	else if (mSelectionBasic)
	{
		// Gère les paramètres de collision
		mSelectionBasic->SetRestitution(mRestitution->GetValue());
	}
}
void BodyScenario::OnClearForces()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Obtient le BasicBody
		myAssert(mSelectionPoly->GetBody(), "Aucun b2Body associé au BasicBody.");
		mSelectionPoly->GetBody()->SetLinearVelocity(b2Vec2_zero);
		mSelectionPoly->GetBody()->SetAngularVelocity(0.f);
	}
	else if (mSelectionBasic)
	{
		// Obtient le BasicBody
		myAssert(mSelectionBasic->GetBody(), "Aucun b2Body associé au BasicBody.");
		mSelectionBasic->GetBody()->SetLinearVelocity(b2Vec2_zero);
		mSelectionBasic->GetBody()->SetAngularVelocity(0.f);
	}
}
void BodyScenario::OnDestroy()
{
	if (!mApply) return;

	if (mSelectionPoly)
	{
		// Détruit le BasicBody
		mEntityMgr.DestroyEntity(mSelectionPoly);
		Unselect();
		mEditBox.ScheduleUnselection();
	}
	else if (mSelectionBasic)
	{
		// Détruit le BasicBody
		mEntityMgr.DestroyEntity(mSelectionBasic);
		Unselect();
		mEditBox.ScheduleUnselection();
	}
}
