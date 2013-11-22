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
	// Fen�tre
	sfg::Desktop &mDesktop;
	sfg::Window::Ptr mWindow;

	// El�ments de la fen�tre
	sfg::Button::Ptr mButton;
};
