#include "stdafx.h"
#include "ColFilteringWindow.h"
#include "../../App/InputManager.h"

// Ctor
ColFilteringWindow::ColFilteringWindow()
	: Window("Collision Filtering"), mSelection(nullptr),
	mLevelMgr(LevelManager::GetInstance()), mPhysicMgr(PhysicManager::GetInstance())
{
	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Gestion de la sélection
void ColFilteringWindow::Select(BaseBody *selection)
{
	myAssert(selection, "BaseBody passé invalide.");

	mSelection = selection;
	SetTitle("Collision Filtering");

	OnRefresh();
}
void ColFilteringWindow::Unselect()
{
	mSelection = nullptr;
	SetTitle("Collision Filtering - Inactif");
}

// Actualisation
void ColFilteringWindow::Update()
{
	// Si on a un BasicBody
	if (!mSelection) return;

	// Récupère les FilterData
	b2Filter fd = mSelection->GetBody()->GetFixtureList()->GetFilterData();

	// Mets à jour le Group Index
	mGroupIndex->SetValue(fd.groupIndex);

	// Mets à jour les Radios
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

// Construit la fenêtre et les éléments
void ColFilteringWindow::Fill()
{
	// Crée les éléments
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
	mColFilteringApply->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ColFilteringWindow::OnColFilteringApply, this));
	mColFilteringRefresh = sfg::Button::Create("Actualiser");
	mColFilteringRefresh->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ColFilteringWindow::OnRefresh, this));
	mColFilteringClose = sfg::Button::Create("Fermer");
	mColFilteringClose->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ColFilteringWindow::OnClose, this));

	// Crée les Radios
	for (int i = 1; i <= 16; ++i)
	{
		// Category Bits
		mCatBits.push_back(sfg::ToggleButton::Create(Parser::intToString(i)));
		// Mask Bits
		mMaskBits.push_back(sfg::ToggleButton::Create(Parser::intToString(i)));

		// Ajoute les radios à la table
		mBitsTable->Attach(mCatBits.back(), sf::Rect<sf::Uint32>(i, 2, 1, 1));
		mBitsTable->Attach(mMaskBits.back(), sf::Rect<sf::Uint32>(i, 4, 1, 1));
	}

	// Attache tous les widgets à la fenêtre
	mGroupIndexBox->PackEnd(mGroupIndexLabel);
	mGroupIndexBox->PackEnd(mGroupIndex);
	mColFilteringButtonsHBox->PackEnd(mColFilteringApply);
	mColFilteringButtonsHBox->PackEnd(mColFilteringRefresh);
	mColFilteringButtonsHBox->PackEnd(mColFilteringClose);
	mColFilteringVBox->PackEnd(mGroupIndexBox);
	mColFilteringVBox->PackEnd(mBitsTable);
	mColFilteringVBox->PackEnd(mColFilteringButtonsHBox);
	AddToWindow(mColFilteringVBox);
}

// Callbacks
void ColFilteringWindow::OnColFilteringApply()
{
	if (!mApply || !mSelection) return;

	// Crée un FilterData
	b2Filter fd;

	// Mets à jour le Group Index
	fd.groupIndex = static_cast<int16>(mGroupIndex->GetValue());

	// Mets à jour les Radios
	uint16 cb = 0;
	uint16 mb = 0;
	for (int i = 1; i <= 16; ++i)
	{
		cb = (cb << 1) | (mCatBits[16 - i]->IsActive() ? 1 : 0);
		mb = (mb << 1) | (mMaskBits[16 - i]->IsActive() ? 1 : 0);
	}
	fd.categoryBits = cb;
	fd.maskBits = mb;

	// Modifie les FilterData
	mSelection->GetBody()->GetFixtureList()->SetFilterData(fd);
}
