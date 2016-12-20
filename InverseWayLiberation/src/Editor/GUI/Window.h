#pragma once
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Widget.hpp>
#include "../../Tools/NonCopyable.h"

class Window : public NonCopyable
{
public:
	// Ctor & Dtor
	Window(const std::string &title);
	virtual ~Window();

	// Obtient la fenêtre
	sfg::Window::Ptr GetWindow();

	// Enregistre la fenêtre dans le Desktop
	void RegisterInDesktop(sfg::Desktop *desktop);
	void UnregisterInDesktop();

	// Actualisation
	virtual void Update() = 0;

	// Gestion de la visibilité
	void Show();
	void Hide();
	bool IsVisible();

protected:
	// Remplit la fenêtre d'éléments
	virtual void Fill() = 0;
	void AddToWindow(sfg::Widget::Ptr widget);

	// Gestion du titre
	void SetTitle(const std::string &title);

	// Callbacks
	void OnRefresh();
	void OnClose();

protected:
	// Applique les modifications
	bool mApply;

private:
	// Etat
	bool mVisible;

	// Fenêtre
	sfg::Desktop *mDesktop;
	sfg::Window::Ptr mWindow;
};
