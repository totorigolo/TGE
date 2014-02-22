#include "stdafx.h"
#include "PolyCreationWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/PolyBody.h"
#include "../../Entities/EntityFactory.h"

// Ctor
PolyCreationWindow::PolyCreationWindow()
	: Window("PolyCreation"),
	mEntityMgr(EntityManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()),
	mIsEnterEditMode(false)
{
	// Rempli la fen�tre
	Fill();
	mApply = true;
}

// Gestion de l'ajout de points
void PolyCreationWindow::AddPoint(b2Vec2 pos)
{
	// Max 8 points
	if (mPoints.size() >= b2_maxPolygonVertices)
		return;

	// Ajoute le point
	mPoints.push_back(pos);

	OnRefresh();
}
void PolyCreationWindow::EmptyPoints()
{
	mPoints.clear();
}
void PolyCreationWindow::DrawPointsOn(sf::RenderTarget &target)
{
	if (!mIsEnterEditMode || mPoints.size() == 0) return;

	// Dessine
	sf::VertexArray edges(sf::LinesStrip, mPoints.size() + 1);
	edges[mPoints.size()].position = b22sfVec(mPoints.front(), mPhysicMgr.GetPPM());

	// Cr�e le disque
	sf::CircleShape cs;

	// Param�tre le cercle
	cs.setFillColor(sf::Color::Yellow);
	cs.setOutlineColor(sf::Color(30, 30, 30));
	cs.setOutlineThickness(1.f);
	cs.setRadius(3.5f);
	cs.setOrigin(sf::Vector2f(3.5f, 3.5f));

	// Parcours chaque point
	for (int i = 0; i < mPoints.size(); ++i)
	{
		cs.setPosition(b22sfVec(mPoints[i], mPhysicMgr.GetPPM()));
		target.draw(cs);

		edges[i].position = cs.getPosition();
	}
	target.draw(edges);
}

// Actualisation
void PolyCreationWindow::Update()
{
	// Max 8 points
	if (mPoints.size() >= b2_maxPolygonVertices && mIsEnterEditMode)
	{
		mMode->SetText("Mode : Cr�ation -> limite atteinte");
	}
}

// Accesseurs
bool PolyCreationWindow::IsInEditMode()
{
	return mIsEnterEditMode;
}

// Construit la fen�tre et les �l�ments
void PolyCreationWindow::Fill()
{
	// Cr�e le Layout
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// El�ments
	mMode = sfg::Label::Create("Mode : Attente");
	mHelpLabel = sfg::Label::Create("Cr�er pts : Ctrl + clic gauche");

	// Type
	mTypeTable = sfg::Table::Create();
	mTypeTable->SetRequisition(sf::Vector2f(0, 30.f));
	mTypeLabel = sfg::Label::Create("Type :");
	mType.resize(2);//(3);
	mType[0] = sfg::RadioButton::Create("Dynamique");
	mType[1] = sfg::RadioButton::Create("Statique", mType[0]->GetGroup());
	//mType[2] = sfg::RadioButton::Create("Kin�matique", mType[1]->GetGroup());
	mTypeTable->Attach(mTypeLabel, sf::Rect<sf::Uint32>(1, 1, 1, 1));
	mTypeTable->Attach(mType[0], sf::Rect<sf::Uint32>(2, 1, 5, 1));
	mTypeTable->Attach(mType[1], sf::Rect<sf::Uint32>(8, 1, 5, 1));

	// Boutons
	mEnterEditModeBtn = sfg::Button::Create("Entrer mode Cr�ation");
	mCreatePolyBtn = sfg::Button::Create("Cr�er poly");
	mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mCancelBtn = sfg::Button::Create("Annuler point");
	mEmptyBtn = sfg::Button::Create("Effacer");
	mCloseBtn = sfg::Button::Create("Fermer");
	mButtonsHBox->PackEnd(mEmptyBtn);
	mButtonsHBox->PackEnd(mCloseBtn);

	// Signaux
	mEnterEditModeBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnEnterEditMode, this));
	mCreatePolyBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnCreatePoly, this));
	mCancelBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnCancelLast, this));
	mEmptyBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnEmptyPoints, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnEmptyPoints, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnClose, this));

	// Ajoute les �l�ments � la fen�tre
	mVBox->PackEnd(mMode);
	mVBox->PackEnd(mHelpLabel);
	mVBox->PackEnd(mTypeTable);
	mVBox->PackEnd(mEnterEditModeBtn);
	mVBox->PackEnd(mCreatePolyBtn);
	mVBox->PackEnd(mCancelBtn);
	mVBox->PackEnd(mButtonsHBox);

	// Ajoute la mVBox � la fen�tre
	AddToWindow(mVBox);
}

// Callbacks
void PolyCreationWindow::OnEmptyPoints()
{
	if (!mApply) return;

	EmptyPoints();
	OnEnterPassiveMode();
}
void PolyCreationWindow::OnCancelLast()
{
	if (!mApply) return;

	// Il faut au minimum un point
	if (mPoints.size() > 0)
		mPoints.pop_back();

	OnRefresh();
}
void PolyCreationWindow::OnCreatePoly()
{
	if (!mApply) return;

	// Il faut au minimum trois points (+1 pour celui du clic)
	if (mPoints.size() < 3) return;

	// Cr�e le Body suivant le type
	if (mType[0]->IsActive()) // Dynamique
	{
		EntityFactory::CreatePolyBody(mPoints, b2BodyType::b2_dynamicBody, "unknown");

		// Supprime les points
		OnEmptyPoints();
	}
	else if (mType[1]->IsActive()) // Statique
	{
		EntityFactory::CreatePolyBody(mPoints, b2BodyType::b2_staticBody, "unknown");

		// Supprime les points
		OnEmptyPoints();
	}
	else
	{
		std::cout << "Aucun type choisi !" << std::endl;
	}
}
void PolyCreationWindow::OnEnterEditMode()
{
	if (!mApply) return;

	mIsEnterEditMode = true;

	mMode->SetText("Mode : Cr�ation");
}
void PolyCreationWindow::OnEnterPassiveMode()
{
	if (!mApply) return;

	mIsEnterEditMode = false;

	mMode->SetText("Mode : Attente");
}
