#pragma once
#include <string>
#include <SFGUI/Window.hpp>
#include <SFGUI/Box.hpp>
#include "../../Tools/NonCopyable.h"

class EditBox;
class Scenario : public NonCopyable
{
public:
	// Ctor & Dtor
	Scenario(EditBox &editBox, const std::string &title);
	virtual ~Scenario();

	// Ajoute la VBox à la fenêtre
	void AddInWindow(sfg::Window::Ptr window);
	void UnregisterInWindow();

	// Actualisation
	virtual void Update() = 0;

	// Gestion de la visibilité
	void Show();
	void Hide();

protected:
	// Remplit le scénario d'éléments
	virtual void Fill() = 0;
	void AddToVBox(sfg::Widget::Ptr widget);

	// Gestion du titre
	void SetTitle(const std::string &title);

	// Callbacks
	void OnRefresh();
	void OnClose();

protected:
	// Applique les modifications
	bool mApply;

	// EditBox
	EditBox &mEditBox;

private:
	// Etat
	bool mVisible;

	// Titre du scénario
	std::string mTitle;

	// Fenêtre
	sfg::Window::Ptr mWindow;

	// Conteneur
	sfg::Box::Ptr mVBox;
};
