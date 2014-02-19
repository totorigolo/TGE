#pragma once
#include "../../Tools/NonCopyable.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <string>

class EditBox;
class Scenario : public NonCopyable
{
public:
	// Ctor & Dtor
	Scenario(EditBox &editBox, const std::string &title);
	virtual ~Scenario();

	// Ajoute la VBox � la fen�tre
	void AddInWindow(sfg::Window::Ptr window);
	void UnregisterInWindow();

	// Actualisation
	virtual void Update() = 0;

	// Gestion de la visibilit�
	void Show();
	void Hide();

protected:
	// Remplit le sc�nario d'�l�ments
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

	// Titre du sc�nario
	std::string mTitle;

	// Fen�tre
	sfg::Window::Ptr mWindow;

	// Conteneur
	sfg::Box::Ptr mVBox;
};
