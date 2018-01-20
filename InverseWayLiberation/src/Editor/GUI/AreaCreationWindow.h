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
#include "../../Level/LevelManager.h"
#include "../../Physics/PhysicManager.h"
#include "../../Entities/EntityManager.h"

class EditBox;

class AreaCreationWindow : public Window {
public:
    // Ctor
    AreaCreationWindow();

    // Gestion de l'ajout
    void Add(b2Vec2 pos);

    // Actualisation
    virtual void Update();

    // Accesseurs
    bool IsInAddMode();

protected:
    // Construit la fenêtre et les éléments
    virtual void Fill();

    // Callbacks
    void OnToggleMode();

private:
    // Etat
    bool mIsInAddMode;

    // Managers
    EntityManager &mEntityMgr;
    PhysicManager &mPhysicMgr;
    ResourceManager &mResourceMgr;
    TriggersManager &mTriggersMgr;

    /* Eléments */
    sfg::Box::Ptr mVBox;
    sfg::Label::Ptr mMode;
    sfg::Label::Ptr mHelpLabel;

    // Position
    sfg::Table::Ptr mDimTable;
    sfg::Label::Ptr mWidthLabel;
    sfg::Entry::Ptr mWidth;
    sfg::Label::Ptr mHeightLabel;
    sfg::Entry::Ptr mHeight;

    // Script
    sfg::Box::Ptr mScriptHBox;
    sfg::Label::Ptr mScriptLabel;
    sfg::ComboBox::Ptr mScript;
    sfg::CheckButton::Ptr mOnce;

    // Boutons
    sfg::Button::Ptr mToggleModeBtn;
    sfg::Box::Ptr mButtonsHBox;
    sfg::Button::Ptr mRefreshBtn;
    sfg::Button::Ptr mCloseBtn;
};
