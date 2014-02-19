#pragma once
#include "../../Tools/NonCopyable.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <string>

class Window : public NonCopyable
{
public:
	// Ctor & Dtor
	Window(const std::string &title);
	virtual ~Window();

	// Obtient la fen�tre
	sfg::Window::Ptr GetWindow();

	// Enregistre la fen�tre dans le Desktop
	void RegisterInDesktop(sfg::Desktop *desktop);
	void UnregisterInDesktop();

	// Actualisation
	virtual void Update() = 0;

	// Gestion de la visibilit�
	void Show();
	void Hide();
	bool IsVisible();

protected:
	// Remplit la fen�tre d'�l�ments
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

	// Fen�tre
	sfg::Desktop *mDesktop;
	sfg::Window::Ptr mWindow;
};
