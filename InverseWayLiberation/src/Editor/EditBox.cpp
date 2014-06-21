#include "stdafx.h"
#include "EditBox.h"
#include "../App/InputManager.h"

#include "../Entities/BasicBody.h"
#include "../Entities/PolyBody.h"
#include "../Entities/PolyChain.h"
#include "../Entities/Deco.h"
#include "../Entities/Grapnel.h"
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
	mUpdateScheduled(false), mUnselectionScheduled(false),
	mLevelMgr(LevelManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()), mEntityMgr(EntityManager::GetInstance()),
	mSelectedEntity(nullptr), mSelectedJoint(nullptr), mSelectionType(SelectionType::Null), mSelectionChanged(false),
	mEmptyScenario(*this), mDecoScenario(*this), mBodyScenario(*this),
	mLevelWindowAdded(false), mLuaConsoleWindowAdded(false), mColFilteringWindowAdded(false), mDecoCreationWindowAdded(false),
	mPolyCreationWindowAdded(false), mBasicBodyCreationWindowAdded(false), mTexturesWindowAdded(false)
{
	// Cr�e la fen�tre
	mWindow = sfg::Window::Create();
	mDesktop.Add(mWindow);
	
	// Change le titre de la fen�tre
	mWindow->SetTitle("EditBox");

	// Affiche le sc�nario par d�faut
	ShowEmptyScenario();
}

// Dtor
EditBox::~EditBox()
{
}

// Actions diff�r�es
void EditBox::DoScheduledTasks()
{
	// Ex�cute les actions programm�es
	if (mUpdateScheduled)
		UpdateGUI();
	if (mUnselectionScheduled)
		Unselect();

	// R�initialise les variables
	mUpdateScheduled = false;
	mUnselectionScheduled = false;
}
void EditBox::ScheduleUpdate()
{
	mUpdateScheduled = true;
}
void EditBox::ScheduleUnselection()
{
	mUnselectionScheduled = true;
}

// Change l'objet actuel
void EditBox::ChangeSelectedObject(Entity *entity)
{
	// Si la s�lection n'a pas chang�, on ne fait rien
	if (entity == mSelectedEntity)
		return;

	// D�selectionne l'objet actuel
	Unselect();

	// Retient l'objet donn�
	myAssert(entity, "L'entity pass�e est invalide.");
	mSelectedEntity = entity;
	mSelectionChanged = true;

	// Mise � jour
	UpdateGUI();
}
void EditBox::ChangeSelectedObject(Joint *joint)
{
	// Si la s�lection n'a pas chang�, on ne fait rien
	if (joint == mSelectedJoint)
		return;

	// D�selectionne l'objet actuel
	Unselect();

	// Retient l'objet donn�
	myAssert(joint, "Le Joint pass� est invalide.");
	mSelectedJoint = joint;
	mSelectionChanged = true;

	// Mise � jour
	UpdateGUI();
}
void EditBox::Unselect()
{
	// Oublie l'Entity
	if (mSelectedEntity)
	{
		mSelectedEntity = nullptr;
		mSelectionChanged = true;
	}

	// Oublie le Joint
	if (mSelectedJoint)
	{
		mSelectedJoint = nullptr;
		mSelectionChanged = true;
	}

	// D�selectionne des diff�rents sc�narios
	mColFilteringWindow.Unselect();

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
	// R�initialise le type
	mSelectionType = SelectionType::Null;

	// Cache les sc�nari
	mDecoScenario.Hide();
	mEmptyScenario.Hide();
	mBodyScenario.Hide();

	// Vide la fen�tre
	mWindow->RemoveAll();
}

// Mise � jour des Widgets en f� de la s�lection
void EditBox::UpdateGUI()
{
	// Si la s�lection n'a pas chang�, on ne fait rien
	if (!mSelectionChanged)
		return;
	mSelectionChanged = false;

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

			// S�lectionne les objets et montre le sc�nario des Body
			mBodyScenario.Select((BasicBody*) e);
			mColFilteringWindow.Select((BasicBody*) e);
			ShowBodyScenario();
		}
		else if (e->GetType() == EntityType::PolyBody)
		{
			mSelectionType = SelectionType::PolyBody;

			// S�lectionne les objets et montre le sc�nario des Body
			mBodyScenario.Select((PolyBody*) e);
			mColFilteringWindow.Select((PolyBody*) e);
			ShowBodyScenario();
		}
		else if (e->GetType() == EntityType::PolyChain)
		{
			mSelectionType = SelectionType::PolyChain;

			// S�lectionne les objets et montre le sc�nario des Body
			mBodyScenario.Select((PolyChain*) e);
			mColFilteringWindow.Select((PolyChain*) e);
			ShowBodyScenario();
		}
		else if (e->GetType() == EntityType::Deco)
		{
			mSelectionType = SelectionType::Deco;

			// Montre le sc�nario de la Deco
			mDecoScenario.Select((Deco*) e);
			ShowDecoScenario();
		}
		else if (e->GetType() == EntityType::Grapnel)
		{
			mSelectionType = SelectionType::Grapnel;
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

	// Affiche le sc�nario par d�faut si on n'a pas de s�lection
	if (mSelectionType == SelectionType::Null)
		ShowEmptyScenario();
}

// Gestion de la Machine Lua
void EditBox::SetLuaMachine(LuaMachine *luaMachine)
{
	myAssert(luaMachine, "La LuaMachine pass�e est invalide.");

	mLuaConsoleWindow.SetLuaMachine(luaMachine);
}
LuaMachine* EditBox::GetLuaMachine()
{
	return mLuaConsoleWindow.GetLuaMachine();
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
	if (mSelectionType == SelectionType::BasicBody || mSelectionType == SelectionType::PolyBody)
	{
		// Obtient le BasicBody
		BaseBody *bb = (BaseBody*) mSelectedEntity;
		myAssert(bb, "Erreur lors du la d�termination du type.");

		cs.setPosition(b22sfVec(bb->GetPosition(), mPhysicMgr.GetPPM()));
	}
	else if (mSelectionType == SelectionType::Deco)
	{
		// Obtient le BasicBody
		Deco *d = (Deco*) mSelectedEntity;
		myAssert(d, "Erreur lors du la d�termination du type.");

		cs.setPosition(d->GetSprite()->getPosition());
	}
	else if (mSelectionType == SelectionType::Player)
	{
		// Obtient le Player
		Player *p = (Player*) mSelectedEntity;
		myAssert(p, "Erreur lors du la d�termination du type.");

		cs.setPosition(b22sfVec(p->GetPosition(), mPhysicMgr.GetPPM()));
	}

	// Retourne le disque
	return cs;
}

// Mise en place des diff�rents sc�narios
void EditBox::ShowEmptyScenario()
{
	mEmptyScenario.AddInWindow(mWindow);
	mEmptyScenario.Show();
}
void EditBox::OnShowLevelWindow()
{
	if (!mLevelWindowAdded)
	{
		mLevelWindow.RegisterInDesktop(&mDesktop);
		mLevelWindowAdded = true;
	}

	mLevelWindow.Show();
}
void EditBox::ShowBodyScenario()
{
	mBodyScenario.AddInWindow(mWindow);
	mBodyScenario.Show();
}
void EditBox::ShowDecoScenario()
{
	mDecoScenario.AddInWindow(mWindow);
	mDecoScenario.Show();
}
void EditBox::ShowTexturesWindow()
{
	if (!mTexturesWindowAdded)
	{
		mTexturesWindow.RegisterInDesktop(&mDesktop);
		mTexturesWindowAdded = true;
	}

	mTexturesWindow.Show();
}
void EditBox::OnShowLuaConsoleWindow()
{
	if (!mLuaConsoleWindowAdded)
	{
		mLuaConsoleWindow.RegisterInDesktop(&mDesktop);
		mLuaConsoleWindowAdded = true;
	}

	mLuaConsoleWindow.Show();
}
void EditBox::OnShowColFilteringWindow()
{
	if (!mColFilteringWindowAdded)
	{
		mColFilteringWindow.RegisterInDesktop(&mDesktop);
		mColFilteringWindowAdded = true;
	}

	mColFilteringWindow.Show();
}
void EditBox::ShowDecoCreationWindow()
{
	if (!mDecoCreationWindowAdded)
	{
		mDecoCreationWindow.RegisterInDesktop(&mDesktop);
		mDecoCreationWindowAdded = true;
	}

	mDecoCreationWindow.Show();
}
void EditBox::ShowPolyCreationWindow()
{
	if (!mLuaConsoleWindowAdded)
	{
		mPolyCreationWindow.RegisterInDesktop(&mDesktop);
		mPolyCreationWindowAdded = true;
	}

	mPolyCreationWindow.Show();
}
void EditBox::ShowBasicBodyCreationWindow()
{
	if (!mBasicBodyCreationWindowAdded)
	{
		mBasicBodyCreationWindow.RegisterInDesktop(&mDesktop);
		mBasicBodyCreationWindowAdded = true;
	}

	mBasicBodyCreationWindow.Show();
}

// Fermeture des fen�tres / sc�narios
void EditBox::OnCloseLevelWindow()
{
	mLevelWindow.Hide();
}

// Accesseurs des fen�tres et sc�narii
EmptyScenario* EditBox::GetEmptyScenario()
{
	return &mEmptyScenario;
}
LevelWindow* EditBox::GetLevelWindow()
{
	return &mLevelWindow;
}
DecoScenario* EditBox::GetDecoScenario()
{
	return &mDecoScenario;
}
BodyScenario* EditBox::GetBodyScenario()
{
	return &mBodyScenario;
}
TexturesWindow* EditBox::GetTexturesWindow()
{
	return &mTexturesWindow;
}
LuaConsoleWindow* EditBox::GetLuaConsoleWindow()
{
	return &mLuaConsoleWindow;
}
ColFilteringWindow* EditBox::GetColFilteringWindow()
{
	return &mColFilteringWindow;
}
DecoCreationWindow* EditBox::GetDecoCreationWindow()
{
	return &mDecoCreationWindow;
}
PolyCreationWindow* EditBox::GetPolyCreationWindow()
{
	return &mPolyCreationWindow;
}
BasicBodyCreationWindow* EditBox::GetBasicBodyCreationWindow()
{
	return &mBasicBodyCreationWindow;
}
