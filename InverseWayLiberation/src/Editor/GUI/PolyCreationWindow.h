#pragma once
#include "Window.h"
#include "../../Entities/BasicBody.h"
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class EditBox;
class PolyCreationWindow : public Window
{
public:
	// Ctor
	PolyCreationWindow();

	// Gestion de l'ajout de points
	void AddPoint(b2Vec2 pos);
	void EmptyPoints();
	void DrawPointsOn(sf::RenderTarget &target);

	// Actualisation
	virtual void Update();

	// Accesseurs
	bool IsInEditMode();

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

	// Callbacks
	void OnEmptyPoints();
	void OnCancelLast();
	void OnCreatePoly();
	void OnEnterEditMode();
	void OnEnterPassiveMode();
	void OnCancelPointCausedByClic();

private:
	// Etat
	bool mIsEnterEditMode;

	// EntityManager, PhysicMgr
	EntityManager &mEntityMgr;
	PhysicManager &mPhysicMgr;

	// Points
	std::vector<b2Vec2> mPoints;

	/* Eléments */
	sfg::Box::Ptr mVBox;
	sfg::Label::Ptr mMode;
	sfg::Label::Ptr mHelpLabel;
	// Type
	sfg::Table::Ptr mTypeTable;
	sfg::Label::Ptr mTypeLabel;
	std::vector<sfg::RadioButton::Ptr> mType;

	// Boutons
	sfg::Button::Ptr mEnterEditModeBtn;
	sfg::Button::Ptr mCreatePolyBtn;
	sfg::Box::Ptr mButtonsHBox;
	sfg::Button::Ptr mCloseBtn;
	sfg::Button::Ptr mCancelBtn;
	sfg::Button::Ptr mEmptyBtn;
};
