#include "EditBox.h"

// Ctor
EditBox::EditBox(sfg::Desktop &desktop)
	: mDesktop(desktop)
{
	// Cr�e la fen�tre
	mWindow = sfg::Window::Create();
	mDesktop.Add(mWindow);

	// Change le titre de la fen�tre
	mWindow->SetTitle("EditBox");

	// Cr�e le bouton
	mButton = sfg::Button::Create("Hello !!!");
	mWindow->Add(mButton);
}

// Dtor
EditBox::~EditBox()
{

}
