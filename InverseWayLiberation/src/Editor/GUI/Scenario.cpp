#include "Scenario.h"
#include "../EditBox.h"
#include "../../Tools/Error.h"

// Ctor
Scenario::Scenario(EditBox &editBox, const std::string &title)
	: mVisible(false), mApply(false), mEditBox(editBox), mTitle(title)
{
	// Crée la VBox
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
}

// Dtor
Scenario::~Scenario()
{
	mApply = false;
}

// Enregistre la fenêtre dans le Desktop
void Scenario::AddInWindow(sfg::Window::Ptr window)
{
	myAssert(window, "Desktop passé invalide.");

	// Retient le Desktop
	mWindow = window;

	// S'enregistre
	mWindow->Add(mVBox);
}
void Scenario::UnregisterInWindow()
{
	mApply = false;

	// Se désenregistre
	if (mWindow)
		mWindow->Remove(mVBox);
}

// Gestion de la visibilité
void Scenario::Show()
{
	mApply = false;
	Update();
	mApply = true;

	// Montre les éléments
	mVBox->Show(true);
	mVisible = true;

	// Change le titre de la fenêtre
	SetTitle(mTitle);
}
void Scenario::Hide()
{
	// Cache les éléments
	mVBox->Show(false);
	mVisible = false;
}

// Remplit la fenêtre d'éléments
void Scenario::AddToVBox(sfg::Widget::Ptr widget)
{
	mVBox->PackEnd(widget);
}

// Gestion du titre
void Scenario::SetTitle(const std::string &title)
{
	if (mWindow)
		mWindow->SetTitle(title);
}

// Callbacks
void Scenario::OnRefresh()
{
	mApply = false;
	Update();
	mApply = true;
}
void Scenario::OnClose()
{
	Hide();
}
