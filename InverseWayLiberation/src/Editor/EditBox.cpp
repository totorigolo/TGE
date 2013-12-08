#include "EditBox.h"
#include "../Tools/Error.h"
#include "../Tools/Parser.h"
#include "../Tools/utils.h"
#include "../App/InputManager.h"

#include "../Entities/BasicBody.h"
#include "../Entities/Deco.h"
#include "../Entities/Grapnel.h"
#include "../Entities/LivingBeing.h"
#include "../Entities/Player.h"

#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Joints/FrictionJoint.h"
#include "../Physics/Joints/GearJoint.h"
#include "../Physics/Joints/PrismaticJoint.h"
#include "../Physics/Joints/PulleyJoint.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/RopeJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Physics/Joints/WheelJoint.h"

#include <iostream>

// Ctor
EditBox::EditBox(sfg::Desktop &desktop)
	: mDesktop(desktop),
	mLevelMgr(LevelManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()),
	mSelectedEntity(nullptr), mSelectedJoint(nullptr), mSelectionType(SelectionType::Null)
{
	// Cr�e la fen�tre
	mWindow = sfg::Window::Create();
	mDesktop.Add(mWindow);

	// Change le titre de la fen�tre
	mWindow->SetTitle("EditBox");

	// Cr�e la VBox
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mWindow->Add(mVBox);

	// Initialise l'EditBox
	CreateEmptyWindow();

	// Initialise des variables
	mPosStepSaveValue = 1.f;
}

// Dtor
EditBox::~EditBox()
{
}

// Change l'objet actuel
void EditBox::ChangeSelectedObject(Entity *entity)
{
	// D�selectionne l'objet actuel
	Unselect();

	// Retient l'objet donn�
	myAssert(entity, "L'entity pass�e est invalide.");
	mSelectedEntity = entity;

	// Mise � jour
	UpdateGUI();
}
void EditBox::ChangeSelectedObject(Joint *joint)
{
	// D�selectionne l'objet actuel
	Unselect();

	// Retient l'objet donn�
	myAssert(joint, "Le Joint pass� est invalide.");
	mSelectedJoint = joint;

	// Mise � jour
	UpdateGUI();
}
void EditBox::Unselect()
{
	// Oublie l'Entity
	if (mSelectedEntity)
		mSelectedEntity = nullptr;

	// Oublie le Joint
	if (mSelectedJoint)
		mSelectedJoint = nullptr;

	// Mise � jour
	UpdateGUI();
}
EditBox::SelectionType EditBox::GetSelectionType()
{
	return mSelectionType;
}
Entity* EditBox::GetSelectedEntity()
{
	return mSelectedEntity;
}
Joint* EditBox::GetSelectedJoint()
{
	return mSelectedJoint;
}
std::string EditBox::SelectionTypeToString(const SelectionType &type)
{
	if (type == SelectionType::Entity)
		return "Entity";
	else if (type == SelectionType::BasicBody)
		return "BasicBody";
	else if (type == SelectionType::Deco)
		return "Deco";
	else if (type == SelectionType::Grapnel)
		return "Grapnel";
	else if (type == SelectionType::LivingBeing)
		return "LivingBeing";
	else if (type == SelectionType::Player)
		return "Player";
	else if (type == SelectionType::Joint)
		return "Joint";
	else if (type == SelectionType::DistanceJoint)
		return "DistanceJoint";
	else if (type == SelectionType::FrictionJoint)
		return "FrictionJoint";
	else if (type == SelectionType::GearJoint)
		return "GearJoint";
	else if (type == SelectionType::PrismaticJoint)
		return "PrismaticJoint";
	else if (type == SelectionType::PulleyJoint)
		return "PulleyJoint";
	else if (type == SelectionType::RevoluteJoint)
		return "RevoluteJoint";
	else if (type == SelectionType::RopeJoint)
		return "RopeJoint";
	else if (type == SelectionType::WeldJoint)
		return "WeldJoint";
	else if (type == SelectionType::WheelJoint)
		return "WheelJoint";
	else
		return "EditBox";
}

// Vide la EditBox
void EditBox::EmptyGUI()
{
	/* Supprime les �l�ments (uniquement ceux dans l'EditBox) */
	// Vide la VBox
	mVBox->RemoveAll();

	// EditBox : Boutons
	mOpenLevelBtn.reset();

	// Entities : Position
	if (mPosTable) mPosTable->RemoveAll();
	mPosTable.reset();
	mPosLabel.reset();
	mPosX.reset();
	mPosY.reset();
	mRot.reset();
	mPosButton.reset();
	if (mPosStep) mPosStepSaveValue = mPosStep->GetValue();
	mPosStep.reset();
	mPosXp.reset();
	mPosXm.reset();
	mPosYp.reset();
	mPosYm.reset();
	mPosRp.reset();
	mPosRm.reset();
	// BasicBody : Type
	if (mTypeTable) mTypeTable->RemoveAll();
	mTypeTable.reset();
	mTypeLabel.reset();
	mType.clear();
	// BasicBody : CollisionType
	if (mTypeTable) mCollisionTypeTable->RemoveAll();
	mCollisionTypeTable.reset();
	mCollisionTypeLabel.reset();
	mCollisionType.reset();
	// BasicBody : Densit�, friction, restitution
	if (mPhysicsParamsTable) mPhysicsParamsTable->RemoveAll();
	mPhysicsParamsTable.reset();
	mDensityLabel.reset();
	mDensity.reset();
	mFrictionLabel.reset();
	mFriction.reset();
	mRestitutionLabel.reset();
	mRestitution.reset();
	// BasicBody : Boutons
	mColFilteringButton.reset();
	mRefresh.reset();
	mButtonsHBox.reset();

	/* Cr�e le sc�nario de fen�tre vide (paradoxe...) */
	CreateEmptyWindow();
}

// Mise � jour des Widgets en f� de l'objet
void EditBox::UpdateGUI()
{
	// Vide la GUI
	EmptyGUI();

	// R�initialise le type
	mSelectionType = SelectionType::Null;

	// Change la GUI en fonction du type d'objet
	if (mSelectedEntity)
	{
		mSelectionType = SelectionType::Entity;

		// R�cup�re un nom raccourci pour l'Entity
		Entity *e = mSelectedEntity;

		// G�re les diff�rents types
		if (e->GetType() == EntityType::BasicBody)
		{
			mSelectionType = SelectionType::BasicBody;

			// Cr�e le sc�nario du BasicBody
			CreateBasicBodyWindow();
		}
		else if (e->GetType() == EntityType::Deco)
		{
			mSelectionType = SelectionType::Deco;
		}
		else if (e->GetType() == EntityType::Grapnel)
		{
			mSelectionType = SelectionType::Grapnel;
		}
		else if (e->GetType() == EntityType::LivingBeing)
		{
			mSelectionType = SelectionType::LivingBeing;
		}
		else if (e->GetType() == EntityType::Player)
		{
			mSelectionType = SelectionType::Player;
		}
	}
	else if (mSelectedJoint)
	{
		// Texte par d�faut
		mSelectionType = SelectionType::Joint;

		// R�cup�re un nom raccourci pour l'Entity
		Joint *e = mSelectedJoint;

		// G�re les diff�rents types
		if (e->GetType() == JointType::DistanceJoint)
		{
			mSelectionType = SelectionType::DistanceJoint;
		}
		else if (e->GetType() == JointType::FrictionJoint)
		{
			mSelectionType = SelectionType::FrictionJoint;
		}
		else if (e->GetType() == JointType::GearJoint)
		{
			mSelectionType = SelectionType::GearJoint;
		}
		else if (e->GetType() == JointType::PrismaticJoint)
		{
			mSelectionType = SelectionType::PrismaticJoint;
		}
		else if (e->GetType() == JointType::PulleyJoint)
		{
			mSelectionType = SelectionType::PulleyJoint;
		}
		else if (e->GetType() == JointType::RevoluteJoint)
		{
			mSelectionType = SelectionType::RevoluteJoint;
		}
		else if (e->GetType() == JointType::RopeJoint)
		{
			mSelectionType = SelectionType::RopeJoint;
		}
		else if (e->GetType() == JointType::WeldJoint)
		{
			mSelectionType = SelectionType::WeldJoint;
		}
		else if (e->GetType() == JointType::WheelJoint)
		{
			mSelectionType = SelectionType::WheelJoint;
		}
	}

	// Change le titre de la fen�tre
	mWindow->SetTitle(SelectionTypeToString(mSelectionType));

	// Ferme la fen�tre de filtrage des collisions si ce n'est pas un BasicBody
	if (mSelectionType != SelectionType::BasicBody && mSelectionType != SelectionType::Null)
		OnColFilteringClose();
	else
		OnColFilteringRefresh();
}

// Obtient le rep�re de l'objet s�lectionn�
sf::CircleShape EditBox::GetSelectionMark()
{
	// Cr�e le disque
	sf::CircleShape cs;

	// Si on n'a pas de s�lection, on renvoi un cercle de rayon nul
	if (mSelectionType == SelectionType::Null)
		return cs;

	// Param�tre le cercle
	cs.setFillColor(sf::Color::Red);
	cs.setOutlineColor(sf::Color(30, 30, 30));
	cs.setOutlineThickness(2.f);
	cs.setRadius(9.f);
	cs.setOrigin(sf::Vector2f(9.f, 9.f));

	// G�re chaque type
	if (mSelectionType == SelectionType::BasicBody)
	{
		// Obtient le BasicBody
		BasicBody *bb = (BasicBody*) mSelectedEntity;
		myAssert(bb, "Aucun BasicBody s�lectionn�.");

		cs.setPosition(b22sfVec(bb->GetPosition(), mPhysicMgr.GetPPM()));
	}

	// Retourne le disque
	return cs;
}

// Mise en place des diff�rents sc�narios
void EditBox::CreateEmptyWindow()
{
	// Change le titre de la fen�tre
	mWindow->SetTitle(SelectionTypeToString(mSelectionType));

	// Widgets
	mOpenLevelBtn = sfg::Button::Create("Fen�tre Level");
	mOpenLevelBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnLevelCreateWindow, this));
	mVBox->PackEnd(mOpenLevelBtn);
}
void EditBox::OnLevelCreateWindow()
{
	// On ne cr�e pas de nouvelle fen�tre si il en existe d�j� une
	if (mLevelWindow.get())
		return;

	// Cr�e la fen�tre
	mLevelWindow = sfg::Window::Create();
	mLevelWindow->SetTitle("Level");
	mDesktop.Add(mLevelWindow);

	// Widgets
	mLevelVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mGravityBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mGravityLabel = sfg::Label::Create("Gravit� :");
	mGravityX = sfg::SpinButton::Create(-2000.f, 2000.f, 0.2f);
	mGravityX->SetDigits(2);
	mGravityY = sfg::SpinButton::Create(-2000.f, 2000.f, 0.2f);
	mGravityY->SetDigits(2);
	mGravityBox->PackEnd(mGravityLabel, false);
	mGravityBox->PackEnd(mGravityX);
	mGravityBox->PackEnd(mGravityY);
	mBckgColorBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mBckgColorLabel = sfg::Label::Create("BckgC :");
	mBckgColorR = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mBckgColorR->SetDigits(0);
	mBckgColorR->SetRequisition(sf::Vector2f(60.f, 0.f));
	mBckgColorG = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mBckgColorG->SetDigits(0);
	mBckgColorG->SetRequisition(sf::Vector2f(60.f, 0.f));
	mBckgColorB = sfg::SpinButton::Create(0.f, 255.f, 1.f);
	mBckgColorB->SetDigits(0);
	mBckgColorB->SetRequisition(sf::Vector2f(60.f, 0.f));
	mBckgColorBox->PackEnd(mBckgColorLabel, false);
	mBckgColorBox->PackEnd(mBckgColorR);
	mBckgColorBox->PackEnd(mBckgColorG);
	mBckgColorBox->PackEnd(mBckgColorB);
	mOriginViewBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mOriginViewLabel = sfg::Label::Create("Origine :");
	mOriginViewX = sfg::SpinButton::Create(-1000000.f, 1000000.f, 1.f);
	mOriginViewX->SetDigits(3);
	mOriginViewY = sfg::SpinButton::Create(-1000000.f, 1000000.f, 1.f);
	mOriginViewY->SetDigits(3);
	mOriginViewCurrentBtn = sfg::Button::Create("C");
	mOriginViewBox->PackEnd(mOriginViewLabel, false);
	mOriginViewBox->PackEnd(mOriginViewX);
	mOriginViewBox->PackEnd(mOriginViewY);
	mOriginViewBox->PackEnd(mOriginViewCurrentBtn, false);
	mDefaultZoomBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mDefaultZoomLabel = sfg::Label::Create("D�f Zoom :");
	mDefaultZoom = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
	mDefaultZoom->SetDigits(3);
	mDefaultZoomCurrentBtn = sfg::Button::Create("C");
	mDefaultZoomBox->PackEnd(mDefaultZoomLabel, false);
	mDefaultZoomBox->PackEnd(mDefaultZoom);
	mDefaultZoomBox->PackEnd(mDefaultZoomCurrentBtn, false);
	mLevelCloseBtn = sfg::Button::Create("Fermer");

	// Met � jour les valeurs
	OnLevelRefresh();

	// Connecte les signaux
	mGravityX->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangeGravityX, this));
	mGravityY->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangeGravityY, this));
	mBckgColorR->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangeBckgColorR, this));
	mBckgColorG->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangeBckgColorG, this));
	mBckgColorB->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangeBckgColorB, this));
	mOriginViewX->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangeOriginViewX, this));
	mOriginViewY->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangeOriginViewY, this));
	mOriginViewCurrentBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnLevelChangeOriginViewCurrent, this));
	mDefaultZoom->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnLevelChangemDefaultZoom, this));
	mDefaultZoomCurrentBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnLevelChangemDefaultZoomCurrent, this));
	mLevelCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnLevelClose, this));

	// Ajoute les �l�ments � la fen�tre
	mLevelVBox->PackEnd(mGravityBox);
	mLevelVBox->PackEnd(mBckgColorBox);
	mLevelVBox->PackEnd(mOriginViewBox);
	mLevelVBox->PackEnd(mDefaultZoomBox);
	mLevelVBox->PackEnd(mLevelCloseBtn);

	// Ajoute la mLevelVBox � la fen�tre
	mLevelWindow->Add(mLevelVBox);

}
void EditBox::CreateBasicBodyWindow()
{
	// Vide l'EditBox
	mVBox->RemoveAll();
	mOpenLevelBtn.reset();

	// R�cup�re un nom raccourci pour le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

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
	mPosButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangePosition, this));
	mPosStep = sfg::SpinButton::Create(0.f, 200.f, 0.1f);
	mPosStep->SetValue(mPosStepSaveValue);
	mPosStep->SetDigits(1);
	mPosStep->SetRequisition(sf::Vector2f(40.f, 0.f));
	mPosXp = sfg::Button::Create("+");
	mPosXp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangePosXp, this));
	mPosXm = sfg::Button::Create("-");
	mPosXm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangePosXm, this));
	mPosYp = sfg::Button::Create("+");
	mPosYp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangePosYp, this));
	mPosYm = sfg::Button::Create("-");
	mPosYm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangePosYm, this));
	mPosRp = sfg::Button::Create("+");
	mPosRp->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangePosRp, this));
	mPosRm = sfg::Button::Create("-");
	mPosRm->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangePosRm, this));
	mPosTable->Attach(mPosLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mPosTable->Attach(mPosX, sf::Rect<sf::Uint32>(2, 1, 4, 1));
	mPosTable->Attach(mPosY, sf::Rect<sf::Uint32>(6, 1, 4, 1));
	mPosTable->Attach(mRot, sf::Rect<sf::Uint32>(10, 1, 4, 1));
	mPosTable->Attach(mPosButton, sf::Rect<sf::Uint32>(14, 1, 1, 1));
	mPosTable->Attach(mPosStep, sf::Rect<sf::Uint32>(1, 2, 1, 1));
	mPosTable->Attach(mPosXp, sf::Rect<sf::Uint32>(2, 2, 2, 1));
	mPosTable->Attach(mPosXm, sf::Rect<sf::Uint32>(4, 2, 2, 1));
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
	//mType[2] = sfg::RadioButton::Create("Kin�matique", mType[1]->GetGroup());
	mType[0]->GetSignal(sfg::ComboBox::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangeType, this));
	mType[1]->GetSignal(sfg::ComboBox::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyChangeType, this));
	mTypeTable->Attach(mTypeLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mTypeTable->Attach(mType[0], sf::Rect<sf::Uint32>(2, 1, 5, 1));
	mTypeTable->Attach(mType[1], sf::Rect<sf::Uint32>(8, 1, 5, 1));

	// CollisionType
	mCollisionTypeTable = sfg::Table::Create();
	mCollisionTypeLabel = sfg::Label::Create("ColType : ");
	mCollisionType = sfg::ComboBox::Create();
	mCollisionType->AppendItem("D�faut");
	mCollisionType->AppendItem("Bullet");
	mCollisionType->AppendItem("OneSidedPlatform");
	mCollisionType->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&EditBox::OnBasicBodyChangeCollisionType, this));
	mCollisionTypeTable->Attach(mCollisionTypeLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mCollisionTypeTable->Attach(mCollisionType, sf::Rect<sf::Uint32>(2, 1, 5, 1));

	// Param�tres de collision
	mPhysicsParamsTable = sfg::Table::Create();
	mDensityLabel = sfg::Label::Create("Densit� :");
	mDensity = sfg::SpinButton::Create(0.f, 100.f, 0.2f);
	mDensity->SetDigits(3);
	mDensity->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnBasicBodyChangeDensity, this));
	mFrictionLabel = sfg::Label::Create("Friction :");
	mFriction = sfg::SpinButton::Create(0.f, 100.f, 0.1f);
	mFriction->SetDigits(3);
	mFriction->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnBasicBodyChangeFriction, this));
	mRestitutionLabel = sfg::Label::Create("Restitution :");
	mRestitution = sfg::SpinButton::Create(0.f, 100.f, 0.1f);
	mRestitution->SetDigits(3);
	mRestitution->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&EditBox::OnBasicBodyChangeRestitution, this));
	mPhysicsParamsTable->Attach(mDensityLabel,		sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mPhysicsParamsTable->Attach(mDensity,			sf::Rect<sf::Uint32>(2, 1, 1, 1));
	mPhysicsParamsTable->Attach(mFrictionLabel,		sf::Rect<sf::Uint32>(1, 2, 1, 1));
	mPhysicsParamsTable->Attach(mFriction,			sf::Rect<sf::Uint32>(2, 2, 1, 1));
	mPhysicsParamsTable->Attach(mRestitutionLabel,	sf::Rect<sf::Uint32>(1, 3, 1, 1));
	mPhysicsParamsTable->Attach(mRestitution,		sf::Rect<sf::Uint32>(2, 3, 1, 1));

	// Bouton d'actualisation & de Collision Masking
	mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mRefresh = sfg::Button::Create("Actualiser");
	mRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnBasicBodyRefresh, this));
	mColFilteringButton = sfg::Button::Create("Col Filtering");
	mColFilteringButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnColFilteringCreateWindow, this));
	mButtonsHBox->PackEnd(mColFilteringButton);
	mButtonsHBox->PackEnd(mRefresh);

	// Ajoute les �l�ments � la fen�tre
	mVBox->PackEnd(mPosTable);
	mVBox->PackEnd(mTypeTable);
	mVBox->PackEnd(mCollisionTypeTable);
	mVBox->PackEnd(mPhysicsParamsTable);
	mVBox->PackEnd(mButtonsHBox);

	// Met � jour les valeurs
	UpdateBasicBodyWindow();
}
void EditBox::OnColFilteringCreateWindow()
{
	// On ne cr�e pas de nouvelle fen�tre si il en existe d�j� une
	if (mColFilteringWindow.get())
		return;

	// Cr�e la fen�tre
	mColFilteringWindow = sfg::Window::Create();
	mColFilteringWindow->SetTitle("Collision Filtering");
	mDesktop.Add(mColFilteringWindow);

	// Cr�e les �l�ments
	mColFilteringVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mGroupIndexBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mColFilteringButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mGroupIndexLabel = sfg::Label::Create("GroupIndex : ");
	mGroupIndex = sfg::SpinButton::Create(-32768.f, 32767.f, 1.f);
	mBitsTable = sfg::Table::Create();
	mCatBitsLabel = sfg::Label::Create("Category Bits :");
	mMaskBitsLabel = sfg::Label::Create("Mask Bits :");
	mBitsTable->Attach(mCatBitsLabel, sf::Rect<sf::Uint32>(1, 1, 16, 1));
	mBitsTable->Attach(mMaskBitsLabel, sf::Rect<sf::Uint32>(1, 3, 16, 1));
	mColFilteringApply = sfg::Button::Create("Appliquer");
	mColFilteringApply->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnColFilteringApply, this));
	mColFilteringRefresh = sfg::Button::Create("Actualiser");
	mColFilteringRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnColFilteringRefresh, this));
	mColFilteringClose = sfg::Button::Create("Fermer");
	mColFilteringClose->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&EditBox::OnColFilteringClose, this));

	// Cr�e les Radios
	for (int i = 1; i <= 16; ++i)
	{
		// Category Bits
		mCatBits.push_back(sfg::ToggleButton::Create(Parser::intToString(i)));
		// Mask Bits
		mMaskBits.push_back(sfg::ToggleButton::Create(Parser::intToString(i)));

		// Ajoute les radios � la table
		mBitsTable->Attach(mCatBits.back(), sf::Rect<sf::Uint32>(i, 2, 1, 1));
		mBitsTable->Attach(mMaskBits.back(), sf::Rect<sf::Uint32>(i, 4, 1, 1));
	}

	// Attache tous les widgets � la fen�tre
	mGroupIndexBox->PackEnd(mGroupIndexLabel);
	mGroupIndexBox->PackEnd(mGroupIndex);
	mColFilteringButtonsHBox->PackEnd(mColFilteringApply);
	mColFilteringButtonsHBox->PackEnd(mColFilteringRefresh);
	mColFilteringButtonsHBox->PackEnd(mColFilteringClose);
	mColFilteringVBox->PackEnd(mGroupIndexBox);
	mColFilteringVBox->PackEnd(mBitsTable);
	mColFilteringVBox->PackEnd(mColFilteringButtonsHBox);
	mColFilteringWindow->Add(mColFilteringVBox);

	// Met � jour les valeurs
	UpdateColFilteringWindow();
}

// Actualise les valeurs des diff�rents sc�narios
void EditBox::UpdateBasicBodyWindow()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Met � jour les valeurs
	mPosX->SetText(Parser::floatToString(bb->GetPosition().x, 4));
	mPosY->SetText(Parser::floatToString(bb->GetPosition().y, 4));
	float rot = bb->GetRotationD(); rot = float(int(rot) % 360) + rot - float(int(rot));
	mRot->SetText(Parser::floatToString(rot, 4));

	// G�re le type
	if (bb->Getb2BodyType() == b2BodyType::b2_dynamicBody)
		mType[0]->SetActive(true);
	else if (bb->Getb2BodyType() == b2BodyType::b2_staticBody)
		mType[1]->SetActive(true);
	else
		myThrowError("Type de BasicBody non g�r�.");

	// G�re le type de collision
	if (bb->GetCollisionType() == BasicBody::CollisionType::Default)
		mCollisionType->SelectItem(0);
	else if (bb->GetCollisionType() == BasicBody::CollisionType::Bullet)
		mCollisionType->SelectItem(1);
	else if (bb->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform)
		mCollisionType->SelectItem(2);
	else
		myThrowError("CollisionType de BasicBody non g�r�.");

	// G�re les param�tres de collision
	mDensity->SetValue(bb->GetDensity());
	mFriction->SetValue(bb->GetFriction());
	mRestitution->SetValue(bb->GetRestitution());
}
void EditBox::UpdateColFilteringWindow()
{
	// Si la fen�tre n'est pas cr��e, on quitte
	if (!mColFilteringWindow.get())
		return;

	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;

	// Si on a un BasicBody
	if (bb)
	{
		// R�cup�re les FilterData
		b2Filter fd = bb->GetBody()->GetFixtureList()->GetFilterData();

		// Mets � jour le Group Index
		mGroupIndex->SetValue(fd.groupIndex);

		// Mets � jour les Radios
		auto cb = fd.categoryBits;
		auto mb = fd.maskBits;
		for (int i = 0; i < 16; ++i)
		{
			mCatBits[i]->SetActive(cb & 1);
			cb = cb >> 1;
			mMaskBits[i]->SetActive(mb & 1);
			mb = mb >> 1;
		}
	}
}

/* Callbacks */
// Level
void EditBox::OnLevelRefresh()
{
	// Gravit�
	mGravityX->SetValue(mPhysicMgr.GetGravity().x);
	mGravityY->SetValue(mPhysicMgr.GetGravity().y);

	// Couleur du fond
	mBckgColorR->SetValue(mLevelMgr.GetBckgColor().r);
	mBckgColorG->SetValue(mLevelMgr.GetBckgColor().g);
	mBckgColorB->SetValue(mLevelMgr.GetBckgColor().b);

	// Origine de la vue
	mOriginViewX->SetValue(mLevelMgr.GetDefaultCenter().x * mPhysicMgr.GetMPP());
	mOriginViewY->SetValue(mLevelMgr.GetDefaultCenter().y * mPhysicMgr.GetMPP());

	// Zoom par d�faut
	mDefaultZoom->SetValue(mLevelMgr.GetDefaultZoom());
}
void EditBox::OnLevelClose()
{
	if (mLevelWindow) mDesktop.Remove(mLevelWindow);
	mLevelVBox.reset();
	mLevelWindow.reset();
	mGravityBox.reset();
	mGravityLabel.reset();
	mGravityX.reset();
	mGravityY.reset();
	mBckgColorBox.reset();
	mBckgColorLabel.reset();
	mBckgColorR.reset();
	mBckgColorG.reset();
	mBckgColorB.reset();
	mOriginViewBox.reset();
	mOriginViewLabel.reset();
	mOriginViewX.reset();
	mOriginViewY.reset();
	mOriginViewCurrentBtn.reset();
	mDefaultZoomBox.reset();
	mDefaultZoomLabel.reset();
	mDefaultZoom.reset();
	mDefaultZoomCurrentBtn.reset();
	mLevelCloseBtn.reset();
}
void EditBox::OnLevelChangeGravityX()
{
	mPhysicMgr.SetGravity(b2Vec2(mGravityX->GetValue(), mPhysicMgr.GetGravity().y));
}
void EditBox::OnLevelChangeGravityY()
{
	mPhysicMgr.SetGravity(b2Vec2(mPhysicMgr.GetGravity().x, mGravityY->GetValue()));
}
void EditBox::OnLevelChangeBckgColorR()
{
	sf::Color c = mLevelMgr.GetBckgColor();
	c.r = static_cast<sf::Uint8>(mBckgColorR->GetValue());
	mLevelMgr.SetBckgColor(c);
}
void EditBox::OnLevelChangeBckgColorG()
{
	sf::Color c = mLevelMgr.GetBckgColor();
	c.g = static_cast<sf::Uint8>(mBckgColorG->GetValue());
	mLevelMgr.SetBckgColor(c);
}
void EditBox::OnLevelChangeBckgColorB()
{
	sf::Color c = mLevelMgr.GetBckgColor();
	c.b = static_cast<sf::Uint8>(mBckgColorB->GetValue());
	mLevelMgr.SetBckgColor(c);
}
void EditBox::OnLevelChangeOriginViewX()
{
	sf::Vector2f c = mLevelMgr.GetDefaultCenter();
	c.x = mOriginViewX->GetValue() * mPhysicMgr.GetPPM();
	mLevelMgr.SetDefaultCenter(c);
}
void EditBox::OnLevelChangeOriginViewY()
{
	sf::Vector2f c = mLevelMgr.GetDefaultCenter();
	c.y = mOriginViewY->GetValue() * mPhysicMgr.GetPPM();
	mLevelMgr.SetDefaultCenter(c);
}
void EditBox::OnLevelChangeOriginViewCurrent()
{
	mLevelMgr.SetDefaultCenter(InputManager::GetInstance().GetCurrentCenter() * mPhysicMgr.GetMPP());

	mOriginViewX->SetValue(mLevelMgr.GetDefaultCenter().x);
	mOriginViewY->SetValue(mLevelMgr.GetDefaultCenter().y);
}
void EditBox::OnLevelChangemDefaultZoom()
{
	mLevelMgr.SetDefaultZoom(mDefaultZoom->GetValue());
}
void EditBox::OnLevelChangemDefaultZoomCurrent()
{
	mDefaultZoom->SetValue(InputManager::GetInstance().GetCurrentZoom());
}
// BasicBodies
void EditBox::OnBasicBodyRefresh()
{
	// Mets � jour les BasicBodies
	UpdateBasicBodyWindow();
}
void EditBox::OnBasicBodyChangePosition()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Change sa position et sa rotation
	bb->SetTransform(b2Vec2(Parser::stringToFloat(mPosX->GetText()), Parser::stringToFloat(mPosY->GetText())),
		Parser::stringToFloat(mRot->GetText()) * RPD);
}
void EditBox::OnBasicBodyChangeType()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	if (mType[0]->IsActive()) bb->Setb2BodyType(b2BodyType::b2_dynamicBody);
	else if (mType[1]->IsActive()) bb->Setb2BodyType(b2BodyType::b2_staticBody);
	//else if (mType[2]->IsActive()) bb->Setb2BodyType(b2BodyType::b2_kinematicBody);
}
void EditBox::OnBasicBodyChangeCollisionType()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	int ntype = mCollisionType->GetSelectedItem();
	if (ntype == 0) bb->SetCollisionType(BasicBody::CollisionType::Default);
	else if (ntype == 1) bb->SetCollisionType(BasicBody::CollisionType::Bullet);
	else if (ntype == 2) bb->SetCollisionType(BasicBody::CollisionType::OneSidedPlatform);
}
void EditBox::OnBasicBodyChangePosXp()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Change sa position
	bb->SetTransform(bb->GetPosition() - b2Vec2(-mPosStep->GetValue(), 0.f), bb->GetRotationR());

	// Actualise
	UpdateBasicBodyWindow();
}
void EditBox::OnBasicBodyChangePosXm()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Change sa position
	bb->SetTransform(bb->GetPosition() - b2Vec2(mPosStep->GetValue(), 0.f), bb->GetRotationR());

	// Actualise
	UpdateBasicBodyWindow();
}
void EditBox::OnBasicBodyChangePosYp()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Change sa position
	bb->SetTransform(bb->GetPosition() - b2Vec2(0.f, -mPosStep->GetValue()), bb->GetRotationR());

	// Actualise
	UpdateBasicBodyWindow();
}
void EditBox::OnBasicBodyChangePosYm()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Change sa position
	bb->SetTransform(bb->GetPosition() - b2Vec2(0.f, mPosStep->GetValue()), bb->GetRotationR());

	// Actualise
	UpdateBasicBodyWindow();
}
void EditBox::OnBasicBodyChangePosRp()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Change sa rotation
	bb->SetTransform(bb->GetPosition(), bb->GetRotationR() - 2*RPD);

	// Actualise
	UpdateBasicBodyWindow();
}
void EditBox::OnBasicBodyChangePosRm()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Change sa position
	bb->SetTransform(bb->GetPosition(), bb->GetRotationR() + 2*RPD);

	// Actualise
	UpdateBasicBodyWindow();
}
void EditBox::OnBasicBodyChangeDensity()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// G�re les param�tres de collision
	bb->SetDensity(mDensity->GetValue());
}
void EditBox::OnBasicBodyChangeFriction()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// G�re les param�tres de collision
	bb->SetFriction(mFriction->GetValue());
}
void EditBox::OnBasicBodyChangeRestitution()
{
	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// G�re les param�tres de collision
	bb->SetRestitution(mRestitution->GetValue());
}
// Collision Filtering
void EditBox::OnColFilteringApply()
{
	// Si la fen�tre n'est pas cr��e, on quitte
	if (!mColFilteringWindow.get())
		return;

	// Obtient le BasicBody
	BasicBody *bb = (BasicBody*) mSelectedEntity;
	myAssert(bb, "Aucun BasicBody s�lectionn�.");

	// Cr�e un FilterData
	b2Filter fd;

	// Mets � jour le Group Index
	fd.groupIndex = static_cast<int16>(mGroupIndex->GetValue());

	// Mets � jour les Radios
	uint16 cb = 0;
	uint16 mb = 0;
	for (int i = 1; i <= 16; ++i)
	{
		std::cout << cb << " | " << (mCatBits[16 - i]->IsActive() ? 1 : 0) << " = " << (cb | (mCatBits[16 - i]->IsActive() ? 1 : 0)) << std::endl;
		cb = (cb << 1) | (mCatBits[16 - i]->IsActive() ? 1 : 0);
		mb = (mb << 1) | (mMaskBits[16 - i]->IsActive() ? 1 : 0);
	}
	std::cout << cb << std::endl;
	fd.categoryBits = cb;
	fd.maskBits = mb;

	// Modifie les FilterData
	bb->GetBody()->GetFixtureList()->SetFilterData(fd);
}
void EditBox::OnColFilteringRefresh()
{
	// Si la fen�tre n'est pas cr��e, on quitte
	if (!mColFilteringWindow.get())
		return;

	// Met � jour les valeurs
	UpdateColFilteringWindow();
}
void EditBox::OnColFilteringClose()
{
	if (mColFilteringWindow) mDesktop.Remove(mColFilteringWindow);
	mColFilteringVBox.reset();
	mColFilteringWindow.reset();
	mGroupIndexBox.reset();
	mGroupIndexLabel.reset();
	mGroupIndex.reset();
	mCatBits.clear();
	mMaskBits.clear();
	mCatBitsLabel.reset();
	mMaskBitsLabel.reset();
	mBitsTable.reset();
	mColFilteringButtonsHBox.reset();
	mColFilteringApply.reset();
	mColFilteringRefresh.reset();
	mColFilteringClose.reset();
}
