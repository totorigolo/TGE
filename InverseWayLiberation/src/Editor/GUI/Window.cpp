#include "stdafx.h"
#include "Window.h"

// Ctor
Window::Window(const std::string &title)
	: mVisible(false), mApply(false), mDesktop(nullptr)
{
	// Crée la fenêtre
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

// Obtient la fenêtre
sfg::Window::Ptr Window::GetWindow()
{
	return mWindow;
}

// Enregistre la fenêtre dans le Desktop
void Window::RegisterInDesktop(sfg::Desktop *desktop)
{
	myAssert(desktop, "Desktop passé invalide.");

	// Retient le Desktop
	mDesktop = desktop;

	// S'enregistre
	mDesktop->Add(mWindow);
}
void Window::UnregisterInDesktop()
{
	mApply = false;

	// Se désenregistre
	if (mDesktop)
		mDesktop->Remove(mWindow);
	mDesktop = nullptr;
}

// Gestion de la visibilité
void Window::Show()
{
	mApply = false;
	Update();
	mApply = true;

	// Montre la fenêtre
	mWindow->Show(true);
	mVisible = true;
}
void Window::Hide()
{
	// Cache la fenêtre
	mWindow->Show(false);
	mVisible = false;
}
bool Window::IsVisible()
{
	return mVisible;
}

// Remplit la fenêtre d'éléments
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
