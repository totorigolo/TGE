#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/SpinButton.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/ComboBox.hpp>
#include "Window.h"
#include "../../Entities/BasicBody.h"

class EditBox; //
class EntityManager; //
class ResourceManager; //
class TriggersManager; //
class ScriptAction; //

class ActionsWindow : public Window {
public:
    // Ctor
    ActionsWindow();

    // Actualisation
    virtual void Update();

protected:
    // Construit la fenêtre et les éléments
    virtual void Fill();

    // Callbacks
    void OnLoadAction(); //
    void OnDeleteAction(); //

private:
    // Managers
    EntityManager &mEntityMgr;
    ResourceManager &mResourceMgr;
    TriggersManager &mTriggersMgr;

    /* Eléments */
    sfg::Box::Ptr mVBox;
    sfg::Label::Ptr mInfosSectionLabel;

    // Chargement de texture
    sfg::Box::Ptr mNameHBox;
    sfg::Label::Ptr mNameLabel;
    sfg::Entry::Ptr mName;
    sfg::Box::Ptr mPathHBox;
    sfg::Label::Ptr mPathLabel;
    sfg::Entry::Ptr mPath;
    sfg::Box::Ptr mFunctionHBox;
    sfg::Label::Ptr mFunctionLabel;
    sfg::Entry::Ptr mFunction;
    sfg::Button::Ptr mCreateBtn;
    sfg::Label::Ptr mCreateLabel;

    // Actions
    sfg::Box::Ptr mActionHBox;
    sfg::Label::Ptr mActionLabel;
    sfg::ComboBox::Ptr mAction;

    // Boutons
    sfg::Box::Ptr mButtonsHBox;
    sfg::Button::Ptr mDeleteBtn;
    sfg::Button::Ptr mRefreshBtn;
    sfg::Button::Ptr mCloseBtn;
};
