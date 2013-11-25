#include "EditBox.h"
#include "../Tools/Error.h"

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

// Ctor
EditBox::EditBox(sfg::Desktop &desktop)
	: mDesktop(desktop),
	mSelectedEntity(nullptr), mSelectedJoint(nullptr)
{
	// Cr�e la fen�tre
	mWindow = sfg::Window::Create();
	mDesktop.Add(mWindow);

	// Change le titre de la fen�tre
	mWindow->SetTitle("EditBox");

	// Cr�e la VBox
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mWindow->Add(mVBox);

	// Cr�e les boutons
	mButton = sfg::Button::Create("Hello,");
	mLabel = sfg::Label::Create("world!");
	mVBox->Pack(mButton);
	mVBox->Pack(mLabel);
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
	Update();
}
void EditBox::ChangeSelectedObject(Joint *joint)
{
	// D�selectionne l'objet actuel
	Unselect();

	// Retient l'objet donn�
	myAssert(joint, "Le Joint pass� est invalide.");
	mSelectedJoint = joint;

	// Mise � jour
	Update();
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
	Update();
}

// Mise � jour des Widgets en f� de l'objet
void EditBox::Update()
{
	// Change le texte
	if (mSelectedEntity)
	{
		// Texte par d�faut
		mLabel->SetText("Entity");

		// R�cup�re un nom raccourci pour l'Entity
		Entity *e = mSelectedEntity;

		// G�re les diff�rents types
		if (e->GetType() == EntityType::BasicBody)
		{
			mLabel->SetText("BasicBody");
		}
		else if (e->GetType() == EntityType::Deco)
		{
			mLabel->SetText("Deco");
		}
		else if (e->GetType() == EntityType::Grapnel)
		{
			mLabel->SetText("Grapnel");
		}
		else if (e->GetType() == EntityType::LivingBeing)
		{
			mLabel->SetText("LivingBeing");
		}
		else if (e->GetType() == EntityType::Player)
		{
			mLabel->SetText("Player");
		}
	}
	else if (mSelectedJoint)
	{
		// Texte par d�faut
		mLabel->SetText("Joint");

		// R�cup�re un nom raccourci pour l'Entity
		Joint *e = mSelectedJoint;

		// G�re les diff�rents types
		if (e->GetType() == JointType::DistanceJoint)
		{
			mLabel->SetText("DistanceJoint");
		}
		else if (e->GetType() == JointType::FrictionJoint)
		{
			mLabel->SetText("FrictionJoint");
		}
		else if (e->GetType() == JointType::GearJoint)
		{
			mLabel->SetText("GearJoint");
		}
		else if (e->GetType() == JointType::PrismaticJoint)
		{
			mLabel->SetText("PrismaticJoint");
		}
		else if (e->GetType() == JointType::PulleyJoint)
		{
			mLabel->SetText("PulleyJoint");
		}
		else if (e->GetType() == JointType::RevoluteJoint)
		{
			mLabel->SetText("RevoluteJoint");
		}
		else if (e->GetType() == JointType::RopeJoint)
		{
			mLabel->SetText("RopeJoint");
		}
		else if (e->GetType() == JointType::WeldJoint)
		{
			mLabel->SetText("WeldJoint");
		}
		else if (e->GetType() == JointType::WheelJoint)
		{
			mLabel->SetText("WheelJoint");
		}
	}
	else
		mLabel->SetText("Aucun objet s�lectionn�.");
}
