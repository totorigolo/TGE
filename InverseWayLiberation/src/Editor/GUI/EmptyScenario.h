#pragma once
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
	// Construit la fen�tre et les �l�ments
	virtual void Fill();

private:
	/* El�ments */
	// Boutons
	sfg::Button::Ptr mOpenLevelBtn;
	sfg::Button::Ptr mOpenLuaConsoleBtn;
	sfg::Button::Ptr mTexturesBtn;
	sfg::Button::Ptr mDecoCreationBtn;
	sfg::Button::Ptr mPolyCreationBtn;
	sfg::Button::Ptr mBasicBodyCreationBtn;
	sfg::Button::Ptr mPointLightCreationBtn;
};
