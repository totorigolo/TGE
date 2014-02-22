#include "stdafx.h"
#include "Window.h"

// Ctor
Window::Window(const std::string &title)
	: mVisible(false), mApply(false), mDesktop(nullptr)
{
	// Cr�e la fen�tre
	mWindow = sfg::Window::Create();
	mWindow->SetTitle(title);
	mWindow->Show(false);
}

// Dtor
Window::~Window()
{
	mApply = false;
	UnregisterInDesktop();
}

// Obtient la fen�tre
sfg::Window::Ptr Window::GetWindow()
{
	return mWindow;
}

// Enregistre la fen�tre dans le Desktop
void Window::RegisterInDesktop(sfg::Desktop *desktop)
{
	myAssert(desktop, "Desktop pass� invalide.");

	// Retient le Desktop
	mDesktop = desktop;

	// S'enregistre
	mDesktop->Add(mWindow);
}
void Window::UnregisterInDesktop()
{
	mApply = false;

	// Se d�senregistre
	if (mDesktop)
		mDesktop->Remove(mWindow);
	mDesktop = nullptr;
}

// Gestion de la visibilit�
void Window::Show()
{
	mApply = false;
	Update();
	mApply = true;

	// Montre la fen�tre
	mWindow->Show(true);
	mVisible = true;
}
void Window::Hide()
{
	// Cache la fen�tre
	mWindow->Show(false);
	mVisible = false;
}
bool Window::IsVisible()
{
	return mVisible;
}

// Remplit la fen�tre d'�l�ments
void Window::AddToWindow(sfg::Widget::Ptr widget)
{
	mWindow->Add(widget);
}

// Gestion du titre
void Window::SetTitle(const std::string &title)
{
	mWindow->SetTitle(title);
}

// Callbacks
void Window::OnRefresh()
{
	mApply = false;
	Update();
	mApply = true;
}
void Window::OnClose()
{
	Hide();
}
