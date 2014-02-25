#include "stdafx.h"
#include "TexturesWindow.h"
#include "../EditBox.h"
#include "../../App/InputManager.h"
#include "../../Entities/PolyBody.h"
#include "../../Entities/EntityFactory.h"

#define CANVAS_SIZE 200.f

// Ctor
TexturesWindow::TexturesWindow()
	: Window("Textures"),
	mEntityMgr(EntityManager::GetInstance()), mResourceMgr(ResourceManager::GetInstance())
{
	// Rempli la fen�tre
	Fill();
	mApply = true;
}

// Actualisation
void TexturesWindow::Update()
{
	// Met � jour les infos sur la texture s�lectionn�e
	if (!mSelectedTexture.expired())
	{
		// Change le nb d'utilisations (1 = TextureMap)
		mNbUsesLabel->SetText("Utilisations : " + Parser::intToString(mSelectedTexture.use_count() - 1));

		// R�cup�re la texture
		auto tex = mSelectedTexture.lock();
		sf::Sprite s(*tex);
		float factor = 1.f;
		if (tex->getSize().x > CANVAS_SIZE || tex->getSize().y > CANVAS_SIZE)
		{
			if (tex->getSize().x > tex->getSize().y)
				factor = CANVAS_SIZE / tex->getSize().x;
			else
				factor = CANVAS_SIZE / tex->getSize().y;
		}
		s.setScale(factor, factor);
		s.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
		s.setPosition(CANVAS_SIZE / 2.f, CANVAS_SIZE / 2.f);

		// Affiche la texture sur le Canvas
		mCanvas->Clear(sf::Color(30, 30, 30));
		mCanvas->Draw(s);
		mCanvas->Display();
	}
	// D�selectionne la texture si le pointeur n'est plus valide
	else
	{
		mSelectedTexture.reset();
		mTexture->SelectItem(sfg::ComboBox::NONE);
	}

	// Mets � jour la liste de texture
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

// Construit la fen�tre et les �l�ments
void TexturesWindow::Fill()
{
	// Cr�e le Layout
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// Labels
	mInfosSectionLabel = sfg::Label::Create("--");

	// Chargement de texture
	mNameHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mNameLabel = sfg::Label::Create("Nom : ");
	mName = sfg::Entry::Create();
	mNameHBox->PackEnd(mNameLabel, false);
	mNameHBox->PackEnd(mName);
	mPathHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mPathLabel = sfg::Label::Create("Chemin : ");
	mPath = sfg::Entry::Create();
	mPathHBox->PackEnd(mPathLabel, false);
	mPathHBox->PackEnd(mPath);
	mLoadBtn = sfg::Button::Create("Chager");
	mLoadLabel = sfg::Label::Create("--");

	// Texture
	mTextureHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mTextureLabel = sfg::Label::Create("Texture : ");
	mTexture = sfg::ComboBox::Create();
	mTextureHBox->PackEnd(mTextureLabel, false);
	mTextureHBox->PackEnd(mTexture);

	// Infos texture
	mNbUsesLabel = sfg::Label::Create("--");
	mCanvas = sfg::Canvas::Create();
	mCanvas->SetRequisition(sf::Vector2f(CANVAS_SIZE, CANVAS_SIZE));

	// Boutons
	mButtonsHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mRefreshBtn = sfg::Button::Create("Actualiser");
	mCloseBtn = sfg::Button::Create("Fermer");
	mButtonsHBox->PackEnd(mRefreshBtn);
	mButtonsHBox->PackEnd(mCloseBtn);

	// Signaux
	mLoadBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&TexturesWindow::OnLoadTexture, this));
	mTexture->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&TexturesWindow::OnSelectTexture, this));
	mRefreshBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&TexturesWindow::OnRefresh, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&TexturesWindow::OnClose, this));

	// Ajoute les �l�ments � la fen�tre
	mVBox->PackEnd(mNameHBox);
	mVBox->PackEnd(mPathHBox);
	mVBox->PackEnd(mLoadLabel);
	mVBox->PackEnd(mLoadBtn);
	mVBox->PackEnd(mInfosSectionLabel);
	mVBox->PackEnd(mTextureHBox);
	mVBox->PackEnd(mNbUsesLabel);
	mVBox->PackEnd(mCanvas);
	mVBox->PackEnd(mButtonsHBox);

	// Ajoute la mVBox � la fen�tre
	AddToWindow(mVBox);
}

// Callbacks
void TexturesWindow::OnLoadTexture()
{
	if (!mApply) return;

	// Si les entr�es sont invalides, on arr�te
	if (mPath->GetText().isEmpty() || mPath->GetText().isEmpty()) return;

	// Charge la texture
	if (mResourceMgr.LoadTexture(mName->GetText(), mPath->GetText()))
		mLoadLabel->SetText("Texture charg�e.");
	else
		mLoadLabel->SetText("Entr�es invalides.");

	OnRefresh();
}
void TexturesWindow::OnSelectTexture()
{
	if (!mApply || mTexture->GetSelectedItem() == sfg::ComboBox::NONE) return;

	// R�cup�re la texture point�e
	mSelectedTexture = mResourceMgr.GetTexture(mTexture->GetItem(mTexture->GetSelectedItem()));

	OnRefresh();
}
