#pragma once
#include "../Entities/Entity.h"
#include "../Physics/Joint.h"
#include "../Level/TriggersManager.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

class EditBox
{
public:
	// Ctor & Dtor
	EditBox(sfg::Desktop &desktop);
	virtual ~EditBox();

	// Change l'objet actuel
	void ChangeSelectedObject(Entity *entity);
	void ChangeSelectedObject(Joint *joint);
	void Unselect();

	// Mise � jour des Widgets en f� de l'objet
	void Update();

private:
	// Fen�tre
	sfg::Desktop &mDesktop;
	sfg::Window::Ptr mWindow;

	// Objet s�lectionn�
	Entity *mSelectedEntity;
	Joint *mSelectedJoint;

	// El�ments de la fen�tre
	sfg::Box::Ptr mVBox;
	sfg::Button::Ptr mButton;
	sfg::Label::Ptr mLabel;
};
