#include "stdafx.h"
#include "PolyCreationWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/PolyBody.h"
#include "../../Entities/PolyChain.h"
#include "../../Entities/EntityFactory.h"

// Ctor
PolyCreationWindow::PolyCreationWindow()
	: Window("PolyCreation"),
	mEntityMgr(EntityManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance()), mResourceMgr(ResourceManager::GetInstance()),
	mIsEnterEditMode(false)
{
	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de l'ajout de points
void PolyCreationWindow::AddPoint(b2Vec2 pos)
{
	// Max 8 points pour les Bodies
	if (mPoints.size() >= b2_maxPolygonVertices && mType[0]->IsActive())
		return;

	// Ajoute le point
	mPoints.push_back(pos);

	OnRefresh();
}
void PolyCreationWindow::EmptyPoints()
{
	mPoints.clear();
}
void PolyCreationWindow::DrawPointsOn(sf::RenderTarget &target, const sf::RenderStates& states)
{
	if (!mIsEnterEditMode || mPoints.size() == 0) return;

	// Dessine
	sf::VertexArray edges(sf::LinesStrip, mPoints.size() + ((mType[1]->IsActive()) ? 0 : 1));
	if (!mType[1]->IsActive())
		edges[mPoints.size()].position = b22sfVec(mPoints.front(), mPhysicMgr.GetPPM());

	// Crée le disque
	sf::CircleShape cs;

	// Paramètre le cercle
	cs.setFillColor(sf::Color::Yellow);
	cs.setOutlineColor(sf::Color(30, 30, 30));
	cs.setOutlineThickness(1.f);
	cs.setRadius(3.5f);
	cs.setOrigin(sf::Vector2f(3.5f, 3.5f));

	// Parcours chaque point
	for (unsigned int i = 0; i < mPoints.size(); ++i)
	{
		cs.setPosition(b22sfVec(mPoints[i], mPhysicMgr.GetPPM()));
		target.draw(cs, states);

		edges[i].position = cs.getPosition();
	}
	target.draw(edges, states);
}

// Actualisation
void PolyCreationWindow::Update()
{
	// Mets à jour le bouton
	if (mIsEnterEditMode)
	{
		mMode->SetText("Mode : Création");
		mEnterEditModeBtn->SetLabel("Sortir mode Création");
	}
	else
	{
		mMode->SetText("Mode : Attente");
		mEnterEditModeBtn->SetLabel("Entrer mode Création");
	}

	// Max 8 points
	if (mPoints.size() == b2_maxPolygonVertices && mIsEnterEditMode && mType[0]->IsActive())
		mMode->SetText("Mode : Création -> limite atteinte");
	else if (mPoints.size() > b2_maxPolygonVertices && mIsEnterEditMode && mType[0]->IsActive())
		mMode->SetText("Mode : Création -> limite atteinte");

	// Mets à jour la liste de texture
	if (mTexture.get())
	{
		auto current = mTexture->GetSelectedItem();
		for (int i = mTexture->GetItemCount(); i > 0; --i)
			mTexture->RemoveItem(i - 1);
		for each (const auto &tex in mResourceMgr.GetTextureMap())
			mTexture->AppendItem(tex.first);
		mTexture->SelectItem(current);
	}

}

// Accesseurs
bool PolyCreationWindow::IsInEditMode()
{
	return mIsEnterEditMode;
}

// Construit la fenêtre et les éléments
void PolyCreationWindow::Fill()
{
	// Crée le Layout
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// Eléments
	mMode = sfg::Label::Create("Mode : Attente");
	mHelpLabel = sfg::Label::Create("Créer pts : Ctrl + clic gauche");

	// Type
	mTypeHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mTypeLabel = sfg::Label::Create("Type :");
	mType.resize(3);
	mType[0] = sfg::RadioButton::Create("Body");
	mType[1] = sfg::RadioButton::Create("Chaine", mType[0]->GetGroup());
	mType[2] = sfg::RadioButton::Create("Loop", mType[1]->GetGroup());
	mTypeHBox->PackEnd(mTypeLabel);
	mTypeHBox->PackEnd(mType[0]);
	mTypeHBox->PackEnd(mType[1]);
	mTypeHBox->PackEnd(mType[2]);
	mType[0]->GetSignal(sfg::RadioButton::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnTypeClicked, this));
	mType[1]->GetSignal(sfg::RadioButton::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnTypeClicked, this));
	mType[2]->GetSignal(sfg::RadioButton::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnTypeClicked, this));

	// Type de Body
	mTypeBodyHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mTypeBodyLabel = sfg::Label::Create("BodyType :");
	mTypeBody.resize(2);//(3);
	mTypeBody[0] = sfg::RadioButton::Create("Dynamique");
	mTypeBody[1] = sfg::RadioButton::Create("Statique", mTypeBody[0]->GetGroup());
	//mTypeBody[2] = sfg::RadioButton::Create("Kinématique", mTypeBody[1]->GetGroup());
	mTypeBodyHBox->PackEnd(mTypeBodyLabel);
	mTypeBodyHBox->PackEnd(mTypeBody[0]);
	mTypeBodyHBox->PackEnd(mTypeBody[1]);

	// Texture
	mTextureHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mTextureLabel = sfg::Label::Create("Texture : ");
	mTexture = sfg::ComboBox::Create();
	mTextureHBox->PackEnd(mTextureLabel, false);
	mTextureHBox->PackEnd(mTexture);

	// Layer
	mLayerHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mLayerLabel = sfg::Label::Create("Layer : ");
	mLayer = sfg::SpinButton::Create(-1000.f, 1000.f, 1.f);
	mLayer->SetValue(1);
	mLayer->SetDigits(0);
	mLayerHBox->PackEnd(mLayerLabel, false);
	mLayerHBox->PackEnd(mLayer);

	// Boutons
	mEnterEditModeBtn = sfg::Button::Create("Entrer mode Création");
	mCreatePolyBtn = sfg::Button::Create("Créer poly");
	mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mCancelBtn = sfg::Button::Create("Annuler point");
	mEmptyBtn = sfg::Button::Create("Effacer");
	mCloseBtn = sfg::Button::Create("Fermer");
	mButtonsHBox->PackEnd(mEmptyBtn);
	mButtonsHBox->PackEnd(mCloseBtn);

	// Signaux
	mEnterEditModeBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnToggleEditMode, this));
	mCreatePolyBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnCreatePoly, this));
	mCancelBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnCancelLast, this));
	mEmptyBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnEmptyPoints, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnEmptyPoints, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&PolyCreationWindow::OnClose, this));

	// Ajoute les éléments à la fenêtre
	mVBox->PackEnd(mMode);
	mVBox->PackEnd(mHelpLabel);
	mVBox->PackEnd(mTypeHBox);
	mVBox->PackEnd(mTypeBodyHBox);
	mVBox->PackEnd(mTextureHBox);
	mVBox->PackEnd(mLayerHBox);
	mVBox->PackEnd(mEnterEditModeBtn);
	mVBox->PackEnd(mCreatePolyBtn);
	mVBox->PackEnd(mCancelBtn);
	mVBox->PackEnd(mButtonsHBox);

	// Ajoute la mVBox à la fenêtre
	AddToWindow(mVBox);
}

// Callbacks
void PolyCreationWindow::OnEmptyPoints()
{
	if (!mApply) return;

	EmptyPoints();
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

	// Il faut au minimum trois points, et max 8 points pour les Bodies
	if (mPoints.size() < 3 || (mPoints.size() > b2_maxPolygonVertices) && mType[0]->IsActive())
	{
		mMode->SetText("Mode : Création -> nb pts invalide");
		return;
	}

	// Il nous faut une texture
	if (mTexture->GetSelectedItem() == sfg::ComboBox::NONE)
	{
		std::cout << "Aucune texture choisie !" << std::endl;
		return;
	}

	// Agit en fx du type
	if (mType[0]->IsActive()) // Body
	{
		// Récupère le type de Body
		b2BodyType type;
		if (mTypeBody[0]->IsActive()) // Dynamique
			type = b2BodyType::b2_dynamicBody;
		else if (mTypeBody[1]->IsActive()) // Statique
			type = b2BodyType::b2_staticBody;
		/*else if (mTypeBody[2]->IsActive()) // Kinématic
		type = b2BodyType::b2_kinematicBody;*/
		else
		{
			std::cout << "Aucun type de Body choisi !" << std::endl;
			return;
		}

		// Crée le Poly
		EntityFactory::CreatePolyBody(mPoints, type, mTexture->GetItem(mTexture->GetSelectedItem()), static_cast<int>(mLayer->GetValue()));
	}
	else if (mType[1]->IsActive()) // Chain
	{
		// Crée la Chaine
		auto type = PolyChain::Type::Chain;
		EntityFactory::CreatePolyChain(mPoints, type, mTexture->GetItem(mTexture->GetSelectedItem()), static_cast<int>(mLayer->GetValue()));
	}
	else if (mType[2]->IsActive()) // Loop
	{
		// Crée la Loop
		auto type = PolyChain::Type::Loop;
		EntityFactory::CreatePolyChain(mPoints, type, mTexture->GetItem(mTexture->GetSelectedItem()), static_cast<int>(mLayer->GetValue()));
	}
	else
	{
		std::cout << "Aucun type choisi !" << std::endl;
		return;
	}

	// Supprime les points
	OnEmptyPoints();

	OnRefresh();
}
void PolyCreationWindow::OnToggleEditMode()
{
	if (!mApply) return;

	mIsEnterEditMode = !mIsEnterEditMode;

	OnRefresh();
}
void PolyCreationWindow::OnTypeClicked()
{
	if (!mApply) return;

	// Si pas Body, pas de BodyType (et inversement)
	mTypeBodyHBox->Show(mType[0]->IsActive());

	OnRefresh();
}
