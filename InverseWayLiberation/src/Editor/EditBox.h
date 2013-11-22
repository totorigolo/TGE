#pragma once
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/SharedPtr.hpp>

class EditBox
{
public:
	// Ctor & Dtor
	EditBox(sfg::Desktop &desktop);
	virtual ~EditBox();

private:
	// Fenêtre
	sfg::Desktop &mDesktop;
	sfg::Window::Ptr mWindow;

	// Eléments de la fenêtre
	sfg::Button::Ptr mButton;
};
