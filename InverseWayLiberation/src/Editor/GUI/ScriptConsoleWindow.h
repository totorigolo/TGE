#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "Window.h"
#include "../../Tools/PromptEntry.h"

class ScriptMachine;

class ScriptConsoleWindow : public Window {
public:
    // Ctor
    ScriptConsoleWindow();

    // Actualisation
    void Update() override;

    // Gestion de la Machine de script
    void SetScriptMachine(ScriptMachine *luaMachine);

    ScriptMachine *GetScriptMachine();

public:
    // Ajoute du texte dans la console
    template<typename T>
    ScriptConsoleWindow &operator<<(const T &text) {
        sfg::Label::Ptr toAdd(sfg::Label::Create(text));
        toAdd->SetAlignment(sf::Vector2f(0.f,0.f));
        mScrolledBox->PackEnd(toAdd);
        autoscroll();
        return *this;
    }

protected:
    // Construit la fenêtre et les éléments
    virtual void Fill();

    // Callbacks
    void OnSend(); //
    void OnExecuteFile(); //
    void OnUp(); //
    void OnDown(); //

    // Auto-scroll
    void autoscroll();

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
    PromptEntry::Ptr mPrompt;
    sfg::Button::Ptr mReturnBtn;
    sfg::Button::Ptr mCloseBtn;
    sfg::ScrolledWindow::Ptr mScrolled;
    sfg::Box::Ptr mScrolledBox;
    sfg::Box::Ptr mPathHBox;
    sfg::Entry::Ptr mPath;
    sfg::Button::Ptr mExecuteFileBtn;
};
