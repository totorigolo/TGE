#include "EditBox.h"

// Ctor
EditBox::EditBox(sfg::Desktop &desktop)
	: mDesktop(desktop)
{
	// Crée la fenêtre
	mWindow = sfg::Window::Create();
	mDesktop.Add(mWindow);

	// Change le titre de la fenêtre
	mWindow->SetTitle("EditBox");

	// Crée le bouton
	mButton = sfg::Button::Create("Hello !!!");
	mWindow->Add(mButton);
}

// Dtor
EditBox::~EditBox()
{

}
