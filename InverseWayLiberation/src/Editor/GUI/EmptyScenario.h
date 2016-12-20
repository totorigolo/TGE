#pragma once

#include <SFGUI/Button.hpp>
#include "Scenario.h"

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
	sfg::Button::Ptr mTexturesBtn;
	sfg::Button::Ptr mDecoCreationBtn;
	sfg::Button::Ptr mPolyCreationBtn;
	sfg::Button::Ptr mBasicBodyCreationBtn;
	sfg::Button::Ptr mPointLightCreationBtn;
	sfg::Button::Ptr mHumCreationBtn;
};
