#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "Window.h"
#include "../../Tools/PromptEntry.h"

class ScriptMachine;

class ScriptConsoleWindow : public Window {
public:
    // Ctor
    ScriptConsoleWindow();

    // Actualisation
    virtual void Update();

    // Gestion de la Machine de script
    void SetScriptMachine(ScriptMachine *luaMachine);

    ScriptMachine *GetScriptMachine();

public:
    // Ajoute du texte dans la console
    template<typename T>
    ScriptConsoleWindow &operator<<(const T &text) {
        mText += text;
        Update();
        return *this;
    }

protected:
    // Construit la fenêtre et les éléments
    virtual void Fill();

    // Callbacks
    void OnSend();

    void OnUp();

    void OnDown();

private:
    // Machine de script
    ScriptMachine *mScriptMachine;

    // Texte de la console
    sf::String mText;

    // Historique
    int mCurrent;
    std::vector<sf::String> mHistory;

    /* Eléments */
    sfg::Box::Ptr mVBox;
    sfg::Box::Ptr mHBox;
    sfg::Label::Ptr mPromptLabel;
    PromptEntry::Ptr mPrompt;
    sfg::Button::Ptr mReturnBtn;
    sfg::Button::Ptr mCloseBtn;
    sfg::ScrolledWindow::Ptr mScrolled;
    sfg::Box::Ptr mScrolledBox;
    sfg::Label::Ptr mTextLabel;
};
