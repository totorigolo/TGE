#pragma once
#include "../Entities/Entity.h"
#include "../Physics/Joint.h"
#include "../Level/TriggersManager.h"
#include "../Tools/NonCopyable.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Game/InputManager.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <string>
#include <vector>

class EditBox : public NonCopyable
{
public:
	enum class SelectionType {
		Null,
		Entity,
			BasicBody,
			Deco,
			Grapnel,
			LivingBeing,
			Player,
		Joint,
			DistanceJoint,
			FrictionJoint,
			GearJoint,
			PrismaticJoint,
			PulleyJoint,
			RevoluteJoint,
			RopeJoint,
			WeldJoint,
			WheelJoint
	};
	std::string SelectionTypeToString(const SelectionType &type);

public:
	// Ctor & Dtor
	EditBox(sfg::Desktop &desktop);
	virtual ~EditBox();

	// Change l'objet actuel
	void ChangeSelectedObject(Entity *entity);
	void ChangeSelectedObject(Joint *joint);
	void Unselect();
	SelectionType GetSelectionType();


	// Vide la EditBox
	void EmptyGUI();

	// Mise à jour des Widgets en f° de l'objet
	void UpdateGUI();

	// Obtient le repère de l'objet sélectionné
	sf::CircleShape GetSelectionMark();

protected:
	// Mise en place des différents scénarios
	void CreateEmptyWindow();
	void OnLevelCreateWindow(); // Callback
	void CreateBasicBodyWindow();
	void OnColFilteringCreateWindow(); // Callback

	// Actualise les valeurs des différents scénarios
	void UpdateLevelWindow();
	void UpdateBasicBodyWindow();
	void UpdateColFilteringWindow();

	/* Callbacks */
	// Level
	void OnLevelRefresh();
	void OnLevelClose();
	void OnLevelChangeGravityX();
	void OnLevelChangeGravityY();
	void OnLevelChangeBckgColorR();
	void OnLevelChangeBckgColorG();
	void OnLevelChangeBckgColorB();
	void OnLevelChangeOriginViewX();
	void OnLevelChangeOriginViewY();
	void OnLevelChangemDefaultZoom();
	// BasicBodies
	void OnBasicBodyRefresh();
	void OnBasicBodyChangePosition();
	void OnBasicBodyChangeType();
	void OnBasicBodyChangeCollisionType();
	void OnBasicBodyChangePosXp();
	void OnBasicBodyChangePosXm();
	void OnBasicBodyChangePosYp();
	void OnBasicBodyChangePosYm();
	void OnBasicBodyChangePosRp();
	void OnBasicBodyChangePosRm();
	void OnBasicBodyChangeDensity();
	void OnBasicBodyChangeFriction();
	void OnBasicBodyChangeRestitution();
	// Collision Filtering
	void OnColFilteringApply();
	void OnColFilteringRefresh();
	void OnColFilteringClose();

private:
	// Fenêtre
	sfg::Desktop &mDesktop;
	sfg::Window::Ptr mWindow;

	// LevelMgr, PhysicMgr, InputMgr
	LevelManager &mLevelMgr;
	PhysicManager &mPhysicMgr;
	InputManager &mInputMgr;

	// Objet sélectionné
	Entity *mSelectedEntity;
	Joint *mSelectedJoint;
	SelectionType mSelectionType;

	/* Eléments */
	// EditBox : VBox
	sfg::Box::Ptr mVBox;

	// EditBox : Boutons
	sfg::Button::Ptr mOpenLevelBtn;

	// Level
	sfg::Window::Ptr mLevelWindow;
	sfg::Box::Ptr mLevelVBox;
	sfg::Box::Ptr mGravityBox;
	sfg::Label::Ptr mGravityLabel;
	sfg::SpinButton::Ptr mGravityX;
	sfg::SpinButton::Ptr mGravityY;
	sfg::Box::Ptr mBckgColorBox;
	sfg::Label::Ptr mBckgColorLabel;
	sfg::SpinButton::Ptr mBckgColorR;
	sfg::SpinButton::Ptr mBckgColorG;
	sfg::SpinButton::Ptr mBckgColorB;
	sfg::Box::Ptr mOriginViewBox;
	sfg::Label::Ptr mOriginViewLabel;
	sfg::SpinButton::Ptr mOriginViewX;
	sfg::SpinButton::Ptr mOriginViewY;
	sfg::Box::Ptr mDefaultZoomBox;
	sfg::Label::Ptr mDefaultZoomLabel;
	sfg::SpinButton::Ptr mDefaultZoom;
	sfg::Button::Ptr mLevelCloseBtn;

	// Entities : Position
	sfg::Table::Ptr mPosTable;
	sfg::Label::Ptr mPosLabel;
	sfg::Entry::Ptr mPosX;
	sfg::Entry::Ptr mPosY;
	sfg::Entry::Ptr mRot;
	sfg::Button::Ptr mPosButton;
	sfg::SpinButton::Ptr mPosStep;
	sfg::Button::Ptr mPosXp;
	sfg::Button::Ptr mPosXm;
	sfg::Button::Ptr mPosYp;
	sfg::Button::Ptr mPosYm;
	sfg::Button::Ptr mPosRp;
	sfg::Button::Ptr mPosRm;
	// BasicBody : Type
	sfg::Table::Ptr mTypeTable;
	sfg::Label::Ptr mTypeLabel;
	std::vector<sfg::RadioButton::Ptr> mType;
	// BasicBody : CollisionType
	sfg::Table::Ptr mCollisionTypeTable;
	sfg::Label::Ptr mCollisionTypeLabel;
	sfg::ComboBox::Ptr mCollisionType;
	// BasicBody : Densité, friction, restitution
	sfg::Table::Ptr mPhysicsParamsTable;
	sfg::Label::Ptr mDensityLabel;
	sfg::SpinButton::Ptr mDensity;
	sfg::Label::Ptr mFrictionLabel;
	sfg::SpinButton::Ptr mFriction;
	sfg::Label::Ptr mRestitutionLabel;
	sfg::SpinButton::Ptr mRestitution;
	// BasicBody : Boutons
	sfg::Button::Ptr mColFilteringButton;
	sfg::Button::Ptr mRefresh;
	sfg::Box::Ptr mButtonsHBox;

	// Collision Filtering
	sfg::Window::Ptr mColFilteringWindow;
	sfg::Box::Ptr mColFilteringVBox;
	sfg::Box::Ptr mGroupIndexBox;
	sfg::Label::Ptr mGroupIndexLabel;
	sfg::SpinButton::Ptr mGroupIndex;
	sfg::Label::Ptr mCatBitsLabel;
	sfg::Label::Ptr mMaskBitsLabel;
	std::vector<sfg::ToggleButton::Ptr> mCatBits;
	std::vector<sfg::ToggleButton::Ptr> mMaskBits;
	sfg::Table::Ptr mBitsTable;
	sfg::Box::Ptr mColFilteringButtonsHBox;
	sfg::Button::Ptr mColFilteringApply;
	sfg::Button::Ptr mColFilteringRefresh;
	sfg::Button::Ptr mColFilteringClose;
};
