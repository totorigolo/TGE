#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/SpinButton.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/ComboBox.hpp>
#include "Scenario.h"
#include "../../Scripts/ScriptArea.h"

class EditBox; //
class ScriptArea; //
class EntityManager; //
class PhysicManager; //
class ResourceManager; //
class TriggersManager; //

class AreaScenario : public Scenario {
public:
    // Ctor
    AreaScenario(EditBox &editBox);

    // Gestion de la sélection
    void Select(ScriptArea *selection); //
    void Unselect(); //

    // Actualisation
    virtual void Update();

protected:
    // Construit la fenêtre et les éléments
    virtual void Fill();

    // Callbacks
    void OnChangePosition(); //
    void OnChangeScript(); //
    void OnChangeOnce(); //
    void OnDestroy(); //

private:
    // Managers
    EntityManager &mEntityMgr;
    PhysicManager &mPhysicMgr;
    ResourceManager &mResourceMgr;
    TriggersManager &mTriggersMgr;

    // Sélection
    ScriptArea *mSelection;

    /* Eléments */
    // Position
    sfg::Table::Ptr mPosTable;
    sfg::Entry::Ptr mPosTop;
    sfg::Entry::Ptr mPosLeft;
    sfg::Entry::Ptr mPosBottom;
    sfg::Entry::Ptr mPosRight;
    sfg::Button::Ptr mPosButton;

    // Script
    sfg::Box::Ptr mScriptHBox;
    sfg::Label::Ptr mScriptLabel;
    sfg::ComboBox::Ptr mScript;
    sfg::CheckButton::Ptr mOnce;

    // Bouton
    sfg::Button::Ptr mRefresh;
    sfg::Button::Ptr mDestroy;
};
