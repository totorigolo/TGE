#pragma once
#include "Scenario.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

class EditBox;
class EmptyScenario : public Scenario
{
public:
	// Ctor
	EmptyScenario(EditBox &editBox);

	// Actualisation
	virtual void Update();

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

private:
	/* Eléments */
	// Boutons
	sfg::Button::Ptr mOpenLevelBtn;
	sfg::Button::Ptr mOpenLuaConsoleBtn;
	sfg::Button::Ptr mPolyCreationBtn;
};
